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

#ifndef _MONITOR_HAMO_H_
#define _MONITOR_HAMO_H_

#include <url_reader.h>
#include <string>

namespace monitor_HaMo
{
  class monitor_HaMo
  {
  public:
    void connect(const std::string & p_HaMo_login,
		 const std::string & p_HaMo_password,
		 bool p_verbose_curl,
		 bool p_verbose_content);

    void manage_proxy(const std::string & p_proxy_host,
		      const std::string & p_proxy_port,
		      const std::string & p_proxy_user,
		      const std::string & p_proxy_password);

    void get_station_data(void);
  private:
    quicky_url_reader::url_reader m_url_reader;
  };
}
#endif // _MONITOR_HAMO_H_
//EOF
