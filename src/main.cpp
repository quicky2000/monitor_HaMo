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

#ifdef _WIN32
#include "curl/curl.h"
#else
#include <curl/curl.h>
#endif // _WIN32
#include "download_buffer.h"
#include "quicky_exception.h"
#include <iostream>
#include <sstream>
#include <cassert>

using namespace quicky_url_reader;

//------------------------------------------------------------------------------
size_t receive_data(void *p_buffer, size_t p_size, size_t p_nmemb, void *p_userp)
{
  size_t l_real_size = p_size * p_nmemb ;
#ifdef DEBUG_URL_READER
  std::cout << "url_reader::receive data of size " << l_real_size << std::endl ;
#endif
  download_buffer * l_buffer = (download_buffer*) p_userp;
  l_buffer->add_data(l_real_size,p_buffer);
  return p_size*p_nmemb;
}


int main(int argc,char ** argv)
{
  int l_status = 0;
  CURL * l_curl_handler = nullptr;

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
      std::string l_post_logging_url  = "https://www.logre.eu/mediawiki/index.php?title=Sp%C3%A9cial:Connexion&action=submitlogin&type=login&returnto=Accueil";
      std::string l_post_fields_begin = "wpLoginAttempt=Se+connecter&wpLoginToken=";
      std::string l_post_fields_end = "&wpName=" + l_user_name_parameter.get_value<std::string>() + "&wpPassword="+ l_user_password_parameter.get_value<std::string>() +"";
      std::string l_login_token_id = "wpLoginToken";
      std::string l_test_page_url = "https://www.logre.eu/wiki/Utilisateur:Pja";
#else
      std::string l_login_url = "https://gride.gr-tsc.com/login";
      std::string l_post_logging_url  = "https://gride.gr-tsc.com/login/auth";
      std::string l_post_fields_begin = "login%5BuserId%5D="+l_user_name_parameter.get_value<std::string>()+"&login%5Bpassword%5D="+l_user_password_parameter.get_value<std::string>()+"&login%5B_csrf_token%5D=";
      std::string l_post_fields_end = "";
      std::string l_login_token_id = "login[_csrf_token]";
      std::string l_test_page_url = "https://gride.gr-tsc.com/service/homemap";
      //std::string l_test_page_url = "https://gride.gr-tsc.com/home";
#endif

      std::cout << "Start" << std::endl ;
  
      // Initialisation of Curl is done by url_reader static instance
      l_curl_handler = curl_easy_init();
        
      // Proxy authentication
      std::string l_proxy_host = l_proxy_host_parameter.value_set() ? l_proxy_host_parameter.get_value<std::string>() : "";
      std::string l_proxy_port = l_proxy_port_parameter.value_set() ? l_proxy_port_parameter.get_value<std::string>() : "";
      std::string l_proxy_user = l_proxy_user_parameter.value_set() ? l_proxy_user_parameter.get_value<std::string>() : "";
      std::string l_proxy_password = l_proxy_password_parameter.value_set() ? l_proxy_password_parameter.get_value<std::string>() : "";

      if("" != l_proxy_host && "" != l_proxy_port && "" != l_proxy_user && "" != l_proxy_password)
        {
          std::cout << "=> Activating proxy authentication" << std::endl ;
          std::string l_proxy = l_proxy_host + ":" + l_proxy_port;
          std::string l_proxy_userpwd = l_proxy_user + ":" + l_proxy_password ;
          curl_easy_setopt(l_curl_handler, CURLOPT_PROXY, l_proxy.c_str());
          curl_easy_setopt(l_curl_handler, CURLOPT_PROXYUSERPWD,l_proxy_userpwd.c_str());
          curl_easy_setopt(l_curl_handler, CURLOPT_SSL_VERIFYPEER, false);
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

      curl_easy_setopt(l_curl_handler, CURLOPT_COOKIESESSION,true); 
      curl_easy_setopt(l_curl_handler, CURLOPT_COOKIEFILE,"toto.txt"); 
      curl_easy_setopt(l_curl_handler,CURLOPT_FOLLOWLOCATION,true);
      download_buffer l_buffer;
      curl_easy_setopt(l_curl_handler, CURLOPT_WRITEFUNCTION,receive_data); 
      curl_easy_setopt(l_curl_handler, CURLOPT_WRITEDATA, (void *)&l_buffer);
      curl_easy_setopt(l_curl_handler, CURLOPT_URL,l_login_url.c_str());
      if(l_verbose_curl_parameter.value_set())
        {
          curl_easy_setopt(l_curl_handler, CURLOPT_VERBOSE,1);
        }

      CURLcode l_curl_status = curl_easy_perform(l_curl_handler);
      if(l_curl_status)
        {
          std::stringstream l_stream;
          l_stream << "Error when downloading \"" << l_login_url << "\" : " << curl_easy_strerror(l_curl_status);
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }

      // Extract login token
      std::string l_content(l_buffer.get_data(),l_buffer.get_size());
      size_t l_pos = l_content.find(l_login_token_id);
      assert(std::string::npos != l_pos);
      l_pos = l_content.find("value",l_pos);
      assert(std::string::npos != l_pos);
      l_pos += std::string("value").size();
      l_pos = l_content.find('"',l_pos);
      assert(std::string::npos != l_pos);
      ++l_pos;
      size_t l_pos_end = l_content.find('"',l_pos);

      if(l_verbose_content_parameter.value_set())
        {
          std::cout << l_content << std::endl ;
        }
      std::string login_token = l_content.substr(l_pos,l_pos_end - l_pos);

      // Connection 
      std::string l_post_fields = l_post_fields_begin + login_token + l_post_fields_end;

      curl_easy_setopt(l_curl_handler, CURLOPT_POST, true);
      curl_easy_setopt(l_curl_handler, CURLOPT_POSTFIELDS,l_post_fields.c_str());
      curl_easy_setopt(l_curl_handler, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
      curl_easy_setopt(l_curl_handler, CURLOPT_URL,l_post_logging_url.c_str());
 
      l_buffer.clear();
      l_curl_status = curl_easy_perform(l_curl_handler);
      if(l_curl_status)
        {
          std::stringstream l_stream;
          l_stream << "Error when downloading \"" << l_post_logging_url << "\" : " << curl_easy_strerror(l_curl_status);
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }

      l_content = std::string(l_buffer.get_data(),l_buffer.get_size());
      if(l_verbose_content_parameter.value_set())
        {
          std::cout << "-------------------------------------------------------------" << std::endl ;
          std::cout << "Content of page after logging" << std::endl;
          std::cout << l_content << std::endl ;
        }
      if(std::string::npos != l_content.find("Incorrect ID or password"))
        {
          throw quicky_exception::quicky_logic_exception("Connection failed !!! Please check your credentials",__LINE__,__FILE__);
        }


      // get some data available only when connected
      std::cout << "-------------------------------------------------------------" << std::endl ;
      std::cout << "Get some page on wiki to check if we are logged" << std::endl ;
      curl_easy_setopt(l_curl_handler, CURLOPT_URL,l_test_page_url.c_str());
      //  curl_easy_setopt(l_curl_handler, CURLOPT_COOKIEFILE,"cookies.txt");
      l_buffer.clear();
      curl_easy_setopt(l_curl_handler, CURLOPT_WRITEFUNCTION,receive_data); 
      l_curl_status = curl_easy_perform(l_curl_handler);
      if(l_curl_status)
        {
          std::stringstream l_stream;
          l_stream << "Error when downloading \"" << l_test_page_url << "\" : " << curl_easy_strerror(l_curl_status);
          throw quicky_exception::quicky_runtime_exception(l_stream.str(),__LINE__,__FILE__);
        }
  
      l_content = std::string(l_buffer.get_data(),l_buffer.get_size());
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
  // Cleaning
  if(l_curl_handler) 
    {
      std::cout << "libcurl handler cleanup" << std::endl ;
      curl_easy_cleanup(l_curl_handler);
      std::cout << "libcurl handler cleanup done" << std::endl ;
      // Global cleanup of Curl is done by url_reader static instance
    }
  return l_status;

}
