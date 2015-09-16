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
#include "parameter_manager.h"

#include "monitor_HaMo.h"
#include "quicky_exception.h"
#include <iostream>



int main(int argc,char ** argv)
{
  int l_status = 0;

  try
    {
      // Defining application command line parameters
      parameter_manager::parameter_manager l_param_manager("monitor_HaMo.exe","--",2);
      parameter_manager::parameter_if l_user_name_parameter("login",false);
      l_param_manager.add(l_user_name_parameter);
      parameter_manager::parameter_if l_user_password_parameter("password",false);
      l_param_manager.add(l_user_password_parameter);

      parameter_manager::parameter_if l_proxy_host_parameter("proxy-host",true);
      l_param_manager.add(l_proxy_host_parameter);
      parameter_manager::parameter_if l_proxy_port_parameter("proxy-port",true);
      l_param_manager.add(l_proxy_port_parameter);
      parameter_manager::parameter_if l_proxy_user_parameter("proxy-user",true);
      l_param_manager.add(l_proxy_user_parameter);
      parameter_manager::parameter_if l_proxy_password_parameter("proxy-password",true);
      l_param_manager.add(l_proxy_password_parameter);

      parameter_manager::parameter_if l_verbose_curl_parameter("verbose-curl",true);
      l_param_manager.add(l_verbose_curl_parameter);
      parameter_manager::parameter_if l_verbose_content_parameter("verbose-content",true);
      l_param_manager.add(l_verbose_content_parameter);

      // Treating parameters
      l_param_manager.treat_parameters(argc,argv);

      std::cout << "Start Monitor_HaMo" << std::endl ;

      monitor_HaMo::monitor_HaMo l_monitor;
  
      // Proxy authentication
      std::string l_proxy_host = l_proxy_host_parameter.value_set() ? l_proxy_host_parameter.get_value<std::string>() : "";
      std::string l_proxy_port = l_proxy_port_parameter.value_set() ? l_proxy_port_parameter.get_value<std::string>() : "";
      std::string l_proxy_user = l_proxy_user_parameter.value_set() ? l_proxy_user_parameter.get_value<std::string>() : "";
      std::string l_proxy_password = l_proxy_password_parameter.value_set() ? l_proxy_password_parameter.get_value<std::string>() : "";

      if("" != l_proxy_host && "" != l_proxy_port && "" != l_proxy_user && "" != l_proxy_password)
        {
          std::cout << "=> Activating proxy authentication" << std::endl ;
	  l_monitor.manage_proxy(l_proxy_host,l_proxy_port,l_proxy_user,l_proxy_password);
        }
      else if("" != l_proxy_host || "" != l_proxy_port || "" != l_proxy_user || "" != l_proxy_password)
        {
          std::string l_error_message = "Incomplete proxy authentication information. The following parameters are undefined : ";
          bool l_previous = false;
          if("" == l_proxy_host) 
            {
              l_previous = true;
              l_error_message += "Proxy host";
            }
          if("" == l_proxy_port) 
            {
              if(l_previous) l_error_message += ", ";
              l_previous = true;
              l_error_message += "Proxy port";
            }
          if("" == l_proxy_user) 
            {
              if(l_previous) l_error_message += ", ";
              l_previous = true;
              l_error_message += "Proxy user";
            }
          if("" == l_proxy_password) 
            {
              if(l_previous) l_error_message += ", ";
              l_previous = true;
              l_error_message += "Proxy password";
            }
          throw quicky_exception::quicky_logic_exception(l_error_message,__LINE__,__FILE__);
        }

      // Connect on Citelib by HaMo website
      l_monitor.connect(l_user_name_parameter.get_value<std::string>(),
			l_user_password_parameter.get_value<std::string>(),
			l_verbose_curl_parameter.value_set(),
			l_verbose_content_parameter.value_set());

      // get some data available only when connected
      std::cout << "-------------------------------------------------------------" << std::endl ;
      std::cout << "Get some data to check if we are logged" << std::endl ;
      l_monitor.get_station_data();
    }
  catch(quicky_exception::quicky_runtime_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      l_status = -1;
    }
  catch(quicky_exception::quicky_logic_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      l_status = -1;
    }
  return l_status;

}
