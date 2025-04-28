#include "include/server.h"

  /**
   * @brief stop server before destruction
   */
  Server::~Server()
  {
    if(is_run)
    {
      Stop();
      while(is_run);
    }
    else
      PrintTerminal("server shutdown");
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
      PrintTerminal("Invalid argument");
    }
  }

    /**
   * @brief Get the Port object
   * 
   * @return short number of listening port
   */
  int Server::GetPort() { return port_; }

  /**
   * @brief start server in separate thread by calling Run()
   */
  void Server::Start()
  {
    if(is_run)
    {
      Stop();
      while(is_run);
    }
    
    thread_ = std::make_shared<std::thread>(&Server::Run, this);
  }

  /**
   * @brief  stop server
   */
  void Server::Stop()
  {
    accp_->close();
    ioc_.stop();
    thread_->join();
    thread_.reset();
    is_run = false;
    PrintTerminal("server shutdown");
  }

  /**
   * @brief start server listening port
   */
  void Server::DoAccept()
  {
    accp_->async_accept(accp_->get_executor(),[this](beast::error_code e, asio::ip::tcp::socket sock)
    {
      if(!e)
      {
        std::make_shared<Session>(std::move(sock))->Run();
        DoAccept(); //allow next client connection
      }
      else if(e != asio::error::operation_aborted)
        WriteLog("Error Accept: " + e.message());
    });
  }

  /**
   * @brief start server
   */
  void Server::Run()
  {
    is_run = true;
    accp_.reset();
    accp_ = std::make_shared<asio::ip::tcp::acceptor>(ioc_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_));
    beast::error_code e;
    WriteLog("Server start. Listen port: " + std::to_string(port_));
    DoAccept();
    ioc_.run();
  }