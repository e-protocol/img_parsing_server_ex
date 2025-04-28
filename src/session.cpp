#include "include/session.h"
#include "include/fileManager.hpp"

  /**
   * @brief Session constructor
   */
  Session::Session(asio::ip::tcp::socket&& sock) : stream_(std::move(sock))
  { 
    ip_ = stream_.socket().remote_endpoint().address().to_string();
    WriteLog("Connected ip: " + ip_);
  }

  /**
   * @brief Session destructor
   */
  Session::~Session()
  {
    DoClose();
    WriteLog("Disconected ip: " + ip_);
  }

  /**
   * @brief method to async read  from socket
   */
  void Session::DoRead()
  {
    req_ = {};
    buf_.clear();
    stream_.expires_after(std::chrono::seconds(30));
    http::async_read(stream_, buf_, req_, 
        beast::bind_front_handler(&Session::OnRead, shared_from_this()));
  }

  /**
   * @brief callback func on async read from socket
   */
  void Session::OnRead(beast::error_code e, size_t bytesTransfered)
  {
    ignore_unused(bytesTransfered);

    if(e == http::error::end_of_stream)
      return DoClose();

    if(e)
    {
      WriteLog("Failed read: " + e.message());
      return;
    }

    DoWrite(HandleRequest(std::move(req_)));
  }

  /**
   * @brief method to async write to socket
   */
  void Session::DoWrite(http::message_generator&& msg)
  {
    bool keepAlive = msg.keep_alive();
    beast::async_write(stream_, std::move(msg), 
        beast::bind_front_handler(&Session::OnWrite, shared_from_this(), keepAlive));
  }

  /**
   * @brief callback func on async write to socket
   */
  void Session::OnWrite(bool keepAlive, beast::error_code e, size_t bytesTransfered)
  {
    boost::ignore_unused(bytesTransfered);

    if(e)
    {
      WriteLog("Failed write: " + e.message());
      return;
    }
    if(!keepAlive)
      return DoClose();
    DoRead();
  }

  /**
   * @brief method to close socket
   */
  void Session::DoClose()
  {
      beast::error_code e;
      stream_.socket().shutdown(asio::ip::tcp::socket::shutdown_both, e);
  }

  /**
   * @brief public method to start listening socket
   */
  void Session::Run() { DoRead();}

  /**
   * @brief method to process request from client
   */
  http::message_generator Session::HandleRequest(http::request<http::string_body>&& req) 
  {
    std::string target(req.target());
    http::response<http::string_body> res;
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.keep_alive(req.keep_alive());
    res.result(http::status::not_found);
    res.body() = "Error: invalid method or target\n";

    if(req.method() != http::verb::post)
      return res;

    res.result(http::status::ok);

    /*if(target == "/info")
      res.body() = "Server status ok";
    else if(target == "/log")
      res.body() = GetLog();
    else if(target == "/upload")*/
    {
      try
      {
        FileManager::processFile(std::move(req));
      }
      catch(const std::exception& e)
      {
        res.body() = "Failed uploading\n";
        WriteLog("Failed uploading from " + ip_ + " error: " + e.what());
      }
      
      res.body() = "File uploaded\n";
    }

    res.prepare_payload();
    return res;
  }