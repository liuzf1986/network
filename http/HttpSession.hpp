#pragma once

#include <functional>

#include "http_parser.h"
#include "TcpConnection.hpp"
#include "HashedWheelTimer.hpp"
#include "Logger.hpp"

using namespace netio;
using namespace std;

class HttpSession {
  typedef shared_ptr<HashedWheelTimeout> SpTimeout;
  typedef weak_ptr<HashedWheelTimeout> WpTimeout;
 public:
  HttpSession(const SpTcpConnection& connection) :
      _parseOffset(0),
      _spConn(connection),
      _timeout()
  {
    http_parser_init(&_parser, HTTP_REQUEST);
    _spConn->setNewMessageHandler(std::bind(&HttpSession::onReceive, this, placeholders::_1, placeholders::_2));
  }

  ~HttpSession() {
  }

  void active() {
    _parser.data = this;
    _spConn->attach();
  }

  void deactive() {
    _spConn->detach();
  }

  /** 
   * generate session id for http session.
   * We use fd as session id.
   * @return 
   */
  uint64_t genSessionId() const {
    return _spConn->getFd()
  }

  /** 
   * reset timeout
   * 
   * @param timeout 
   */
  void resetTimeout(const SpTimeout& timeout) {
    if(SpTimeout spTimeout = _timeout.lock()) {
      spTimeout->cancel();
    }
    _timeout = timeout;
  }

  /** 
   * connection OnNewMessage callback.
   * 
   * @param connection 
   * @param buffer 
   */
  void onReceive(SpTcpConnection connection, SpVecBuffer& buffer) {
    size_t parsed = http_parser_execute(&_parser, &_parserSetting, buffer->readablePtr(), buffer->readableSize());
    if(0 != _parser.http_errno) {
      LOGW("hts", "%s parsed failed, str=%s", connection->strInfo(), http_errno_name(_parser.http_errno));
    } else {
      _parseOffset += parsed;
    }
  }

  

 private:
  // we mark read when message is complete to garentee buffer is continuos,
  // so we got appending information _offset to indecate where we have parsed.
  size_t _parseOffset;
  SpTcpConnection _spConn;
  http_parser _parser;
  http_parser_url _parserUrl;
  
  WpTimeout _timeout;
};

