#pragma once
#include <vector>
#include <iostream>
#include <atomic>
#include <memory>
#include "session.h"
#include <thread>

/**
* @brief Tinny http server class
* @details Listen port for incoming http data transfer,
* expected data: jpg image
*/
class Server : public Log
{
  int port_ = 8080;
  std::atomic<bool> is_run = false;
  std::shared_ptr<asio::ip::tcp::acceptor> accp_;
  asio::io_context ioc_;
  std::shared_ptr<std::thread> thread_;
  
public:
  ~Server();
  void SetPort(const std::vector<std::string>& args);
  int GetPort();
  void Start();
  void Stop();

private:
  void DoAccept();
  void DoClose();
  void Run();
};