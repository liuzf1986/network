#include "HttpSession.hpp"

http_parser_settings HttpSession::_setting = {
  .on_message_begin = &HttpSession::message_begin_callback,
  .on_url = &HttpSession::url_callback,
  .on_status = &HttpSession::status_callback,
  .on_header_field = &HttpSession::header_field_callback,
  .on_header_value = &HttpSession::header_value_callback,
  .on_headers_complete = &HttpSession::headers_complete_callback,
  .on_body = &HttpSession::body_callback,
  .on_message_complete = &HttpSession::message_complete_callback
};

HttpSession::HttpSession(const SpTcpConnection& connection) :
    _parseOffset(0),
    _spConn(connection),
    _holder(this, HTTP_REQUEST),
    _timeout()
{
  _holder.reset();
  _spConn->setNewMessageHandler(std::bind(&HttpSession::onReceive, this, placeholders::_1, placeholders::_2));
}

HttpSession::~HttpSession() {
}

void HttpSession::active() {
  _spConn->attach();
}

void HttpSession::deactive() {
  _spConn->detach();
}








