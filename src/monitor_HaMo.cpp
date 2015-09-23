/*    This file is part of monitor_Hamo
      Copyright (C) 2015  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "monitor_HaMo.h"
#include "json_parser.h"
#include <iostream>
#include <sstream>
#include <ctime>

namespace monitor_HaMo
{
  //----------------------------------------------------------------------------
  void monitor_HaMo::manage_proxy(const std::string & p_proxy_host,
				  const std::string & p_proxy_port,
				  const std::string & p_proxy_user,
				  const std::string & p_proxy_password)
  {
    m_url_reader.set_authentication(p_proxy_host,
				    p_proxy_port,
				    p_proxy_user,
				    p_proxy_password,
				    true);
  }
  
  //----------------------------------------------------------------------------
  void monitor_HaMo::connect(const std::string & p_HaMo_login,
			     const std::string & p_HaMo_password,
			     bool p_verbose_curl,
			     bool p_verbose_content)
  {
    std::string l_login_url = "https://gride.gr-tsc.com/login";
    std::string l_post_login_url  = "https://gride.gr-tsc.com/login/auth";
    std::string l_post_fields_begin = "login%5BuserId%5D=" + p_HaMo_login + "&login%5Bpassword%5D=" + p_HaMo_password + "&login%5B_csrf_token%5D=";
    std::string l_post_fields_end = "";
    std::string l_login_token_id = "login[_csrf_token]";
    m_url_reader.connect(l_login_url,
			 l_post_login_url,
			 l_post_fields_begin,
			 l_post_fields_end,
			 l_login_token_id,
			 p_verbose_curl,
			 p_verbose_content);
  }

  //----------------------------------------------------------------------------
  void monitor_HaMo::get_station_data(void)
  {
    std::string l_content;

    time_t l_time = time(nullptr);
    std::stringstream l_stream;
    l_stream << l_time;
m_url_reader.dump_url("https://gride.gr-tsc.com/service/homemap?_="+l_stream.str(),l_content);

    std::string l_formated_content;
    std::string l_prefix;
    for(unsigned int l_index = 0 ; l_index < l_content.size() ; ++l_index)
      {
	l_formated_content += l_content[l_index];
	if(',' == l_content[l_index])
	  {
	    l_formated_content += '\n' + l_prefix;
	  }
	if('{' == l_content[l_index] || '[' == l_content[l_index])
	  {
	    l_prefix += ' ';
	    l_formated_content += '\n' + l_prefix;
	  }
	if('}' == l_content[l_index] || ']' == l_content[l_index])
	  {
	    l_prefix = l_prefix.substr(1);
	    l_formated_content += '\n' + l_prefix;
	  }
      }
    std::cout << l_formated_content << std::endl ;

    json_kit::json_parser l_parser;
    const json_kit::json_object & l_data = l_parser.parse(l_content);

    if(!l_data.contains("zone")) throw quicky_exception::quicky_logic_exception("Expecting \"zone\" member in returned JSON object",__LINE__,__FILE__);

    const json_kit::json_value & l_stations = l_data.get("zone").get("areas").get(0).get("stations");
    unsigned int l_station_number = l_stations.get_size();
    uint64_t l_total_coms = 0;
    uint64_t l_total_iroads = 0;
    uint64_t l_total_parking = 0;
    size_t l_max_size = 0;
    for(unsigned int l_index = 0 ; l_index < l_station_number ; ++l_index)
    {
      size_t l_size = l_stations.get(l_index).get("stationName").get_size();
      if(l_size > l_max_size) l_max_size = l_size;
    }
    for(unsigned int l_index = 0 ; l_index < l_station_number ; ++l_index)
    {
      const json_kit::json_value & l_station = l_stations.get(l_index);
      const json_kit::json_value & l_station_json_name = l_station.get("stationName");
      std::string l_station_name = l_station_json_name.get_string();
      if(l_station_json_name.get_size() < l_max_size)
      {
        l_station_name += std::string(l_max_size - l_station_json_name.get_size(),' ');
      }
      uint64_t l_nb_parking = l_station.get("garage").get_uint64_t();
      l_total_parking += l_nb_parking;
      std::cout << l_station_name << " : " << l_nb_parking << " places, " << l_station.get("ev").get_uint64_t() << " vehicules dont ";
      const json_kit::json_value & l_car_details = l_station.get("evByType");
      uint64_t l_nb_coms = l_car_details.get("1").get_uint64_t();
      uint64_t l_nb_iroads = l_car_details.get("2").get_uint64_t();
      l_total_coms += l_nb_coms;
      l_total_iroads += l_nb_iroads;
      std::cout << l_nb_coms  << " Coms et " << l_nb_iroads << " Iroads" << std::endl ;
    }
    std::cout << "Soit " << l_total_coms << " Coms, " << l_total_iroads << " Iroads, " << l_total_parking << " places reparties sur " << l_station_number << " stations" << std::endl;
    delete(&l_data);

  }
  
}
//EOF

