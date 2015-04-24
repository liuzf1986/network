#pragma once

#include <functional>

#include "http_parser.h"
#include "TcpConnection.hpp"
#include "HashedWheelTimer.hpp"
#include "Logger.hpp"

using namespace netio;
using namespace std;

enum class ParseState {
  ParseHeader,
  ParseBody,
  ParseDone
};

class HttpSession {
  typedef shared_ptr<HashedWheelTimeout> SpTimeout;
  typedef weak_ptr<HashedWheelTimeout> WpTimeout;
 public:
  HttpSession(const SpTcpConnection& connection, http_parser_settings* settingPtr);
  ~HttpSession();

  void active();
  void deactive();

  /** 
   * generate session id for http session.
   * We use fd as session id.
   * @return 
   */
  uint64_t genSessionId() const {
    return _spConn->getFd();
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
    size_t parsed = http_parser_execute(&_parser,
                                        _settingPtr,
                                        reinterpret_cast<char*>(buffer->readablePtr() + _parseOffset),
                                        buffer->readableSize() - _parseOffset);
    if(0 != _parser.http_errno) {
      LOGW("hts", "%s parsed failed, str=%s", connection->strInfo(), http_errno_name(static_cast<http_errno>(_parser.http_errno)));
    } else {
      _parseOffset += parsed;
      if(ParseState::ParseDone == _parseState) {
        buffer->markRead(_parseOffset);
        _parseOffset = 0;
        resetParser();
      }
    }
  }

  void updateParseState(const ParseState& parseState) {
    _parseState = parseState;
  }

  SpTcpConnection& getConnection() {
    return _spConn;
  }

  bool onUrl(const char *buf, size_t buflen) {
    if(0 == http_parser_parse_url(buf, buflen, 0, &_parserUrl)) {
      return true;
    }
    return false;
  }

 private:
  void resetParser() {
    http_parser_init(&_parser, HTTP_REQUEST);
    _parser.data = this;
  }
  
  // we mark read when message is complete to garentee buffer is continuos,
  // so we got appending information _offset to indecate where we have parsed.
  size_t _parseOffset;
  ParseState _parseState;
  
  SpTcpConnection _spConn;
  
  http_parser _parser;
  http_parser_url _parserUrl;
  http_parser_settings* _settingPtr;
  
  WpTimeout _timeout;
};





