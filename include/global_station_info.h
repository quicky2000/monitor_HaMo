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

#ifndef _GLOBAL_STATION_INFO_H_
#define _GLOBAL_STATION_INFO_H_

#include "station_info.h"
#include "quicky_exception.h"
#include <map>
#include <string>
#include <iostream>
#include <cassert>

namespace monitor_HaMo
{
  class global_station_info
  {
    inline friend std::ostream & operator<<(std::ostream & p_stream,const global_station_info & p_info);
  public:
    inline global_station_info(void);
    inline void add_station_info(const std::string & p_station_name,
				 const unsigned int & p_station_name_size,
				 const station_info & p_station_info);
    inline void clear(void);
  private:
    station_info m_total_station_info;
    typedef std::map<std::string,station_info> t_stations_info;
    t_stations_info m_stations_info;
    typedef std::map<std::string,unsigned int> t_stations_name_info;
    t_stations_name_info m_stations_name_info;
    unsigned int m_max_name_length;
  };

  //----------------------------------------------------------------------------
  global_station_info::global_station_info(void):
    m_total_station_info(0,0,0,0),
    m_max_name_length(0)
  {
  }

  //----------------------------------------------------------------------------
  void global_station_info::add_station_info(const std::string & p_station_name,
					     const unsigned int & p_station_name_size,
					     const station_info & p_station_info)
  {
    t_stations_info::const_iterator l_info_iter = m_stations_info.find(p_station_name);
    if(m_stations_info.end() != l_info_iter)
      {
	throw quicky_exception::quicky_logic_exception("Station \"" + p_station_name + "\" is already present in global info",__LINE__,__FILE__);	
      }
    m_stations_info.insert(t_stations_info::value_type(p_station_name,p_station_info));
    m_stations_name_info.insert(t_stations_name_info::value_type(p_station_name,p_station_name_size));
    if(p_station_name_size > m_max_name_length)
      {
	m_max_name_length = p_station_name_size;
      }
    m_total_station_info.add_nb_car(p_station_info.get_nb_car());
    m_total_station_info.add_nb_coms(p_station_info.get_nb_coms());
    m_total_station_info.add_nb_iroad(p_station_info.get_nb_iroad());
    m_total_station_info.add_nb_parking(p_station_info.get_nb_parking());
  }

  //----------------------------------------------------------------------------
  void global_station_info::clear(void)
  {
    m_total_station_info.set_nb_car(0);
    m_total_station_info.set_nb_coms(0);
    m_total_station_info.set_nb_iroad(0);
    m_total_station_info.set_nb_parking(0);
    m_max_name_length = 0;
    m_stations_info.clear();
    m_stations_name_info.clear();
  }

  //----------------------------------------------------------------------------
  std::ostream & operator<<(std::ostream & p_stream,const global_station_info & p_info)
  {
    for(auto l_station_iter :  p_info.m_stations_info)
    {
      global_station_info::t_stations_name_info::const_iterator l_name_info_iter = p_info.m_stations_name_info.find(l_station_iter.first);
      assert(m_stations_name_info.end() != l_name_info_iter);
      std::string l_station_name = l_station_iter.first + std::string(p_info.m_max_name_length - l_name_info_iter->second,' ');
      p_stream << l_station_name << " : " << l_station_iter.second << std::endl;
    }
    p_stream << "Soit " << p_info.m_total_station_info << " repartis sur " << p_info.m_stations_info.size() << " stations" << std::endl ;
    return p_stream;
  }
}
#endif // _GLOBAL_STATION_INFO_H_
//EOF
