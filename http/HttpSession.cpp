#include "HttpSession.hpp"

HttpSession::HttpSession(const SpTcpConnection& connection, http_parser_settings* settingPtr) :
    _parseOffset(0),
    _parseState(ParseState::ParseDone),
    _spConn(connection),
    _settingPtr(settingPtr),
    _timeout()
{
  _spConn->setNewMessageHandler(std::bind(&HttpSession::onReceive, this, placeholders::_1, placeholders::_2));
}

HttpSession::~HttpSession() {
}

void HttpSession::active() {
  resetParser();        
  _spConn->attach();
}

void HttpSession::deactive() {
  _spConn->detach();
}
