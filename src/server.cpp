#include "include/server.h"
#include "include/fileManager.hpp"

/**
 * @brief stop server before destruction
 */
Server::~Server()
{
  if(is_run)
    server_.get()->Stop();
}

/**
   * @brief Set the Port object, value validation
   * 
   * @param args command splitted into arguments
   */
  void Server::SetPort(const std::vector<std::string>& args)
  {
    try
    {
      port_ = std::stoi(args[1]);
    }
    catch(const std::exception& e)
    {
      printf("Invalid argument\n");
    }
  }

  /**
   * @brief Set the Host object, uses ipv4 validation
   * 
   * @param args command splitted into arguments
   */
  void Server::SetHost(std::vector<std::string>& args)
  {
    bool ok = ValidateIp(args[1]);
    
    if(!ok)
    {
      printf("Invalid argument\n");
      return;
    }

    host_ = args[1];
  }

    /**
   * @brief Get the Port object
   * 
   * @return short number of listening port
   */
  int Server::GetPort() { return port_; }

  /**
   * @brief static func to validate ipv4, remove first zeros
   * 
   * @return true if ok and false if any symbol is non-numeric
   */
  bool Server::ValidateIp(std::string& ip)
  {
    std::vector<std::string> ip_num;

    for(int i = 0, j = 0; i < ip.size(); ++i)
      if(ip[i] == '.' || i + 1 == ip.size())
      {
        ip_num.push_back(ip.substr(j, i));
        j = i + 1;
      }

    if(ip_num.size() != 4)
      return false;

    std::string correct_ip;

    for(int i = 0; i < ip_num.size(); ++i)
    {
      int num;
      try
      {
        num = std::atoi(ip_num[i].c_str());

        if(num < 0 || num > 255)
          throw std::bad_cast();
      }
      catch(std::exception& e)
      {
        return false;
      }

      correct_ip += std::to_string(num);
      if(i + 1 != ip_num.size()) correct_ip += '.';
    }
    std::swap(ip, correct_ip);
    return true;
  }

  /**
   * @brief Get the Host object
   * 
   */
  void Server::GetHost(){ std::cout << host_ << std::endl; }

  /**
   * @brief start server, required registered handler before server starts for processing client requests
   */
  void Server::Start()
  {
    if(is_run)
      return;
    
    using namespace simple_http_server;
    printf("start server...host: %s port: %d\n", host_.c_str(), port_);
    server_.reset();
    server_ = std::make_shared<HttpServer>(host_, port_);
    is_run = true;
    auto resp_func = [this](const HttpRequest& request) -> HttpResponse 
    { return ProcessRequest(request); };

    server_.get()->RegisterHttpRequestHandler("/", HttpMethod::POST, resp_func);

    try
    {
      server_.get()->Start();
    }
    catch(const std::exception& e)
    {
      printf("%s", e.what());
      is_run = false;
    }
  }

  /**
   * @brief stop server
   */
  void Server::Stop()
  {
    is_run = false;
    printf("server stoped\n");

    try
    {
      server_.get()->Stop();
    }
    catch(const std::exception& e)
    {
      printf("%s", e.what());
    }
  }

  /**
   * @brief method for processing client request
   */
  simple_http_server::HttpResponse Server::ProcessRequest(const simple_http_server::HttpRequest& req)
  {
    //save file
    //std::string path;
    //FileManager::saveFile(req.content().c_str(), path);

    //process image
    //FaceDetectorImg fd(path);
    //fd.Process();

    //response to client
    simple_http_server::HttpResponse response(simple_http_server::HttpStatusCode::Ok);
    std::string content;
    response.SetHeader("Content-Type", "text/plain");
    response.SetContent(content);
    return response;
  }