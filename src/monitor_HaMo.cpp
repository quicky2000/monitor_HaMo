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
#include <iostream>

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
    m_url_reader.dump_url("https://gride.gr-tsc.com/service/homemap",l_content);

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
  }
  
}
//EOF

