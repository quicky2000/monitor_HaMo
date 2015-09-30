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

#ifndef _STATION_INFO_H_
#define _STATION_INFO_H_

#include <iostream>

namespace monitor_HaMo
{
  class station_info
  {
    inline friend std::ostream & operator<<(std::ostream & p_stream,const station_info & p_info);
  public:
    inline station_info(const unsigned int & p_nb_car,
			const unsigned int & p_nb_coms,
			const unsigned int & p_nb_iroad,
			const unsigned int & p_nb_parking);
    inline const unsigned int & get_nb_car(void)const;
    inline void add_nb_car(const unsigned int & p_nb_car);
    inline const unsigned int & get_nb_coms(void)const;
    inline void add_nb_coms(const unsigned int & p_nb_coms);
    inline const unsigned int & get_nb_iroad(void)const;
    inline void add_nb_iroad(const unsigned int & p_nb_iroad);
    inline const unsigned int & get_nb_parking(void)const;
    inline void add_nb_parking(const unsigned int & p_nb_parking);
    inline void set_nb_car(const unsigned int & p_nb_car);
    inline void set_nb_coms(const unsigned int & p_nb_coms);
    inline void set_nb_iroad(const unsigned int & p_nb_iroad);
    inline void set_nb_parking(const unsigned int & p_nb_parking);
  private:
    unsigned int m_nb_car;
    unsigned int m_nb_coms;
    unsigned int m_nb_iroad;
    unsigned int m_nb_parking;
  };

  //----------------------------------------------------------------------------
  station_info::station_info(const unsigned int & p_nb_car,
			     const unsigned int & p_nb_coms,
			     const unsigned int & p_nb_iroad,
			     const unsigned int & p_nb_parking):
    m_nb_car(p_nb_car),
    m_nb_coms(p_nb_coms),
    m_nb_iroad(p_nb_iroad),
    m_nb_parking(p_nb_parking)
  {
  }

  //----------------------------------------------------------------------------
  const unsigned int & station_info::get_nb_car(void)const
  {
    return m_nb_car;
  }

  //----------------------------------------------------------------------------
  const unsigned int & station_info::get_nb_iroad(void)const
  {
    return m_nb_iroad;
  }

  //----------------------------------------------------------------------------
  const unsigned int & station_info::get_nb_coms(void)const
  {
    return m_nb_coms;
  }

  //----------------------------------------------------------------------------
  const unsigned int & station_info::get_nb_parking(void)const
  {
    return m_nb_parking;
  }

  //----------------------------------------------------------------------------
  void station_info::add_nb_car(const unsigned int & p_nb_car)
  {
    m_nb_car += p_nb_car;
  }

  //----------------------------------------------------------------------------
  void station_info::add_nb_coms(const unsigned int & p_nb_coms)
  {
    m_nb_coms += p_nb_coms;
  }

  //----------------------------------------------------------------------------
  void station_info::add_nb_iroad(const unsigned int & p_nb_iroad)
  {
    m_nb_iroad += p_nb_iroad;
  }

  //----------------------------------------------------------------------------
  void station_info::add_nb_parking(const unsigned int & p_nb_parking)
  {
    m_nb_parking += p_nb_parking;
  }

  //----------------------------------------------------------------------------
  void station_info::set_nb_car(const unsigned int & p_nb_car)
  {
    m_nb_car = p_nb_car;
  }

  //----------------------------------------------------------------------------
  void station_info::set_nb_coms(const unsigned int & p_nb_coms)
  {
    m_nb_coms = p_nb_coms;
  }

  //----------------------------------------------------------------------------
  void station_info::set_nb_iroad(const unsigned int & p_nb_iroad)
  {
    m_nb_iroad = p_nb_iroad;
  }

  //----------------------------------------------------------------------------
  void station_info::set_nb_parking(const unsigned int & p_nb_parking)
  {
    m_nb_parking = p_nb_parking;
  }

  //----------------------------------------------------------------------------
  std::ostream & operator<<(std::ostream & p_stream,const station_info & p_info)
  {
    p_stream << p_info.m_nb_parking << " places, " << p_info.m_nb_car << " vehicules dont " << p_info.m_nb_coms  << " Coms et " << p_info.m_nb_iroad << " Iroads";
    return p_stream;
  }
}
#endif // _STATION_INFO_H_
//EOF
