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

#include "url_reader.h"
#include "quicky_exception.h"
#include <iostream>

using namespace quicky_url_reader;


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
#if 0
      std::string l_login_url = "https://www.logre.eu/mediawiki/index.php?title=Sp%C3%A9cial:Connexion&returnto=Accueil";
      std::string l_post_login_url  = "https://www.logre.eu/mediawiki/index.php?title=Sp%C3%A9cial:Connexion&action=submitlogin&type=login&returnto=Accueil";
      std::string l_post_fields_begin = "wpLoginAttempt=Se+connecter&wpLoginToken=";
      std::string l_post_fields_end = "&wpName=" + l_user_name_parameter.get_value<std::string>() + "&wpPassword="+ l_user_password_parameter.get_value<std::string>() +"";
      std::string l_login_token_id = "wpLoginToken";
      std::string l_test_page_url = "https://www.logre.eu/wiki/Utilisateur:Pja";
#else
      std::string l_login_url = "https://gride.gr-tsc.com/login";
      std::string l_post_login_url  = "https://gride.gr-tsc.com/login/auth";
      std::string l_post_fields_begin = "login%5BuserId%5D="+l_user_name_parameter.get_value<std::string>()+"&login%5Bpassword%5D="+l_user_password_parameter.get_value<std::string>()+"&login%5B_csrf_token%5D=";
      std::string l_post_fields_end = "";
      std::string l_login_token_id = "login[_csrf_token]";
      std::string l_test_page_url = "https://gride.gr-tsc.com/service/homemap";
#endif

      std::cout << "Start" << std::endl ;


      quicky_url_reader::url_reader l_url_reader;
  
      // Proxy authentication
      std::string l_proxy_host = l_proxy_host_parameter.value_set() ? l_proxy_host_parameter.get_value<std::string>() : "";
      std::string l_proxy_port = l_proxy_port_parameter.value_set() ? l_proxy_port_parameter.get_value<std::string>() : "";
      std::string l_proxy_user = l_proxy_user_parameter.value_set() ? l_proxy_user_parameter.get_value<std::string>() : "";
      std::string l_proxy_password = l_proxy_password_parameter.value_set() ? l_proxy_password_parameter.get_value<std::string>() : "";

      if("" != l_proxy_host && "" != l_proxy_port && "" != l_proxy_user && "" != l_proxy_password)
        {
          std::cout << "=> Activating proxy authentication" << std::endl ;
	  l_url_reader.set_authentication(l_proxy_host,l_proxy_port,l_proxy_user,l_proxy_password);
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


      l_url_reader.connect(l_login_url,
			   l_post_login_url,
			   l_post_fields_begin,
			   l_post_fields_end,
			   l_login_token_id,
			   l_verbose_curl_parameter.value_set(),
			   l_verbose_content_parameter.value_set());

      // get some data available only when connected
      std::cout << "-------------------------------------------------------------" << std::endl ;
      std::cout << "Get some data to check if we are logged" << std::endl ;

      std::string l_content;
      l_url_reader.dump_url("https://gride.gr-tsc.com/service/homemap",l_content);

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
