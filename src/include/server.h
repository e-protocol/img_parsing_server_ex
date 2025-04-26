#pragma once
#include <vector>
#include <iostream>
#include <atomic>
#include <memory>
#include "../thirdparty/http_server/http_server.h"
#include "imgProcess.hpp"


/**
* @brief Tinny http server class
* @details Listen port for incoming http data transfer,
* expected data: jpg image
*/
class Server
{
  int port_ = 8080;
  std::string host_ = "0.0.0.0";
  std::atomic<bool> is_run = false;
  std::shared_ptr<simple_http_server::HttpServer> server_;
public:
  ~Server();
  void SetPort(const std::vector<std::string>& args);
  void SetHost(std::vector<std::string>& args);
  int GetPort();
  static bool ValidateIp(std::string& ip);
  void GetHost();
  void Start();
  void Stop();

private:
simple_http_server::HttpResponse ProcessRequest(const simple_http_server::HttpRequest& req);
};