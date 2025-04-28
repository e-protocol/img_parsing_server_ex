#pragma once
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "log.hpp"
#include "imgProcess.hpp"

using namespace boost;
using namespace beast;

class Session : public std::enable_shared_from_this<Session>, public Log
{
  std::string ip_;
  http::request<http::string_body> req_;
  beast::flat_buffer buf_;
  beast::tcp_stream stream_;
 
public:
  Session(asio::ip::tcp::socket&& sock);
  ~Session();
  void Run();

private:
  void DoRead();
  void OnRead(beast::error_code e, size_t bytesTransfered);
  void DoWrite(http::message_generator&& msg);
  void OnWrite(bool keepAlive, beast::error_code e, size_t bytesTransfered);
  void DoClose();
  http::message_generator HandleRequest(http::request<http::string_body>&& req);
};
