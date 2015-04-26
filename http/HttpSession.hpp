#pragma once

#include <functional>
#include <string>
#include <strings.h>

#include "http_parser.h"
#include "TcpConnection.hpp"
#include "HashedWheelTimer.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "HttpMessage.hpp"

using namespace netio;
using namespace std;



class HttpSession {
  typedef shared_ptr<HashedWheelTimeout> SpTimeout;
  typedef weak_ptr<HashedWheelTimeout> WpTimeout;
  
  typedef function<void(HttpSession&, const HttpMessageHolder&)> MessageHandler;
  typedef function<void(HttpSession&, int)> ErrorHandler;
 public:
  HttpSession(const SpTcpConnection& connection);
  ~HttpSession();

  void active();
  void deactive();

  /** 
   * generate session id for http session.
   * We use fd as session id.
   * @return 
   */
  int genSessionId() const {
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
    size_t parsed = _holder.consume(&_setting,
                                    reinterpret_cast<char*>(buffer->readablePtr() + _parseOffset),
                                    buffer->readableSize() - _parseOffset);

    if(0 != _holder.getErrno()) {
      _onError(*this, _holder.getErrno());
    } else {
      _parseOffset += parsed;
      if(_holder.isMessageDone()) {
        _onMessage(*this, _holder);
        
        // after message processed, mark buffer readed.
        buffer->markRead(_parseOffset);
        _parseOffset = 0;
        _holder.reset();
      }
    }
  }

  SpTcpConnection getConnection() const {
    return _spConn;
  }

  const HttpMessageHolder& holder() const {
    return _holder;
  }

  HttpMessageHolder& holder() {
    return _holder;
  }

  void setMessageHandler(const MessageHandler& handler) {
    _onMessage = handler;
  }

  void setMessageHandler(MessageHandler&& hanlder) {
    _onMessage = std::move(hanlder);
  }

  void setErrorHandler(const ErrorHandler& handler) {
    _onError = handler;
  }

  void setErrorHandler(ErrorHandler&& handler) {
    _onError = std::move(handler);
  }

  MultiplexLooper* getLooper() const {
    return _spConn->getLooper();
  }

 private:
  
  // callbacks for http_parser
  static http_parser_settings _setting;  
  static int message_begin_callback(http_parser* parser);
  static int url_callback(http_parser* parser, const char *at, size_t length);
  static int status_callback(http_parser* parser, const char *at, size_t length);
  static int header_field_callback(http_parser* parser, const char *at, size_t length);
  static int header_value_callback(http_parser* parser, const char *at, size_t length);
  static int headers_complete_callback(http_parser* parser);
  static int body_callback(http_parser* parser, const char *at, size_t length);
  static int message_complete_callback(http_parser* parser);
  
  // we mark read when message is complete to garentee buffer is continuos,
  // so we got appending information _offset to indecate where we have parsed.
  size_t _parseOffset;

  HttpMessageHolder _holder;
  SpTcpConnection _spConn;
  WpTimeout _timeout;

  // session callback
  MessageHandler _onMessage;
  ErrorHandler _onError;
};


inline int HttpSession::message_begin_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setState(ParseState::ParseHeader);
  return 0;
}

inline int HttpSession::url_callback(http_parser* parser, const char *at, size_t length) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  return sessionPtr->holder().setUrl(at, length);
}

inline int HttpSession::status_callback(http_parser* parser, const char *at, size_t length) {
  return 0;
}

inline int HttpSession::header_field_callback(http_parser* parser, const char *at, size_t length) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setHeaderField(at, length);
  return 0;
}

inline int HttpSession::header_value_callback(http_parser* parser, const char *at, size_t length) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setHeaderValue(at, length);
  return 0;
}

inline int HttpSession::headers_complete_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setState(ParseState::ParseBody);
  return 0;
}

inline int HttpSession::body_callback(http_parser* parser, const char *at, size_t length) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setBody(at, length);
  return 0;
}

inline int HttpSession::message_complete_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->holder().setState(ParseState::ParseDone);
  return 0;    
}


