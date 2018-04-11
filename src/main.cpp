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
#include "global_station_info.h"
#include "password_input.h"
#include <iostream>
#include <chrono>
#include <ctime>

int main(int argc,char ** argv)
{
  int l_status = 0;
  try
    {
      // Defining application command line parameters
      parameter_manager::parameter_manager l_param_manager("monitor_HaMo.exe","--",1);
      parameter_manager::parameter_if l_user_name_parameter("login",false);
      l_param_manager.add(l_user_name_parameter);
      parameter_manager::parameter_if l_user_password_parameter("password",true);
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

      parameter_manager::parameter_if l_refresh_delay_parameter("refresh-delay",true);
      l_param_manager.add(l_refresh_delay_parameter);

      // Treating parameters
      l_param_manager.treat_parameters(argc,argv);
      std::string l_user_name = l_user_name_parameter.value_set() ? l_user_name_parameter.get_value<std::string>() : "";
      std::string l_user_password = l_user_password_parameter.value_set() ? l_user_password_parameter.get_value<std::string>() : "";

      unsigned int l_delay = l_refresh_delay_parameter.value_set() ? atoi(l_refresh_delay_parameter.get_value<std::string>().c_str()) : 60;
      if("" == l_user_password)
      {
        std::string l_prompt = "enter password for " + l_user_name + "\n";
        getpass2( l_user_password, l_prompt);
      }

      std::cout << "Start Monitor_HaMo" << std::endl ;

      monitor_HaMo::monitor_HaMo l_monitor;

      // Proxy authentication
      std::string l_proxy_host = l_proxy_host_parameter.value_set() ? l_proxy_host_parameter.get_value<std::string>() : "";
      std::string l_proxy_port = l_proxy_port_parameter.value_set() ? l_proxy_port_parameter.get_value<std::string>() : "";
      std::string l_proxy_user = l_proxy_user_parameter.value_set() ? l_proxy_user_parameter.get_value<std::string>() : "";
      std::string l_proxy_password = l_proxy_password_parameter.value_set() ? l_proxy_password_parameter.get_value<std::string>() : "";

      if("" != l_proxy_host && "" != l_proxy_port && ("" == l_proxy_user || "" == l_proxy_password))
      {
        if ("" == l_proxy_user)
        {
          l_proxy_user = getlogin();
        }
        if ("" == l_proxy_password)
        {
          std::string l_prompt = "enter proxy password for " + l_proxy_user + "@" + l_proxy_host + ":" + l_proxy_port + "\n";
          getpass2( l_proxy_password, l_prompt);
        }
      }
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
      l_monitor.connect(l_user_name,
                        l_user_password,
                        l_verbose_curl_parameter.value_set(),
                        l_verbose_content_parameter.value_set());

      // get some data available only when connected
      std::cout << "-------------------------------------------------------------" << std::endl ;
      std::cout << "Get some data to check if we are logged" << std::endl ;

      monitor_HaMo::global_station_info l_info;
      monitor_HaMo::global_station_info l_info2;
      monitor_HaMo::global_station_info * l_current_info = &l_info;
      monitor_HaMo::global_station_info * l_past_info = &l_info2;
      while(1)
	{
	  std::chrono::time_point<std::chrono::system_clock> l_time = std::chrono::system_clock::now();
	  std::time_t l_end_time = std::chrono::system_clock::to_time_t(l_time);
	  std::string l_string_time = std::ctime(&l_end_time);
	  l_string_time = l_string_time.substr(0,l_string_time.size() - 1);
	  std::cout.flush();
	  std::cout << "\r" << std::string(50,' ') << "\rCheck at " << l_string_time << "";
	  std::cout.flush();

	  l_monitor.get_station_data(*l_current_info);
	  if(*l_current_info != *l_past_info)
	    {
	      std::cout << std::endl << "Changements observes :" << std::endl ;
	      l_past_info->diff_report(std::cout,*l_current_info);
	      std::cout << "Nouveau status : " << std::endl ;
	      std::cout << *l_current_info << std::endl << std::endl ;
	    }
	  std::swap(l_current_info,l_past_info);

#ifndef _WIN32
	  sleep(l_delay);
#else
	  Sleep(1000*l_delay);
#endif

	}
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
