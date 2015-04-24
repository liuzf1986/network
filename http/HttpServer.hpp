#pragma once

#include <memory>
#include <map>

#include "http_parser.h"

#include "TcpServer.hpp"
#include "TcpConnection.hpp"
#include "HashedWheelTimer.hpp"
#include "TimerWrap.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "VecBuffer.hpp"
#include "HttpSession.hpp"

using namespace netio;

#ifdef __cplusplus
extern "C" {
#endif
int message_begin_callback(http_parser* parser);
int url_callback(http_parser* parser, const char *at, size_t length);
int status_callback(http_parser* parser, const char *at, size_t length);
int header_field_callback(http_parser* parser, const char *at, size_t length);
int header_value_callback(http_parser* parser, const char *at, size_t length);
int headers_complete_callback(http_parser* parser);
int body_callback(http_parser* parser, const char *at, size_t length);
int message_complete_callback(http_parser* parser);
#ifdef __cplusplus
}
#endif

class HttpServer {
  enum { TimerInterval = 100 };    
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLoopPool;
  typedef shared_ptr<HttpSession> SpSession;
  typedef std::map<int, SpSession> SessionMap;
  typedef shared_ptr<HashedWheelTimeout> SpTimeout;
 public:
  HttpServer(uint16_t port, SpLooperPool loopPool, uint32_t keepAliveMS, uint32_t idleTimeoutMS) :
      _keepAliveMS(keepAliveMS),
      _idleTimeoutMS(idleTimeoutMS),
      _server(port, loopPool),
      _timer(loopPool->getLooper(), TimerInterval, _keepAliveMS / TimerInterval),
      _sessionMap()
  {
    _server.setMessageHandler(std::bind(&HttpServer::onReceive, this, placeholders::_1, placeholders::_2));
    _server.setConnectionHandler(std::bind(&HttpServer::onNewConnection, this, placeholders::_1));
    http_parser_settings_init(&_parserSetting);
    _parserSetting.on_url = url_callback;
    _parserSetting.on_body = body_callback;
    _parserSetting.on_status = status_callback;
    _parserSetting.on_header_value = header_value_callback;

  }

  void startWork() {
    _timer.attach();
    _server.startWork();
  }

  void stopWork() {
    _server.stopWork();
    _timer.detach();
  }
  
  
 private:
  /** 
   * New connection trigger by tcpsrever, create and save new session.
   * 
   * @param connection 
   */
  void onNewConnection(SpTcpConnection& connection) {
    SpSession spSession = SpSession(new HttpSession(connection));
    _sessionMap.insert(std::pair<int, SpSession>(spSession->genSessionId(), spSession));
    SpTimeout timeout = _timer.addTimeout(std::bind(&HttpServer::removeSession, this, spSession->genSessionId()), _idleTimeoutMS);
    spSession->resetTimeout(timeout);

    connection->attach();
  }

  /** 
   * remove session by session id
   * 
   * @param sid 
   */
  void removeSession(int sid) {
    auto iter = _sessionMap.find(sid);
    if(iter != _sessionMap.end()) {
      _server.removeConnection((*iter).second);
    }
    _sessionMap.erase(sid);
  }
  

  uint32_t _keepAliveMS;
  uint32_t _idleTimeoutMS;
  TcpServer _server;
  TimerWrap<HashedWheelTimer> _timer;
  SessionMap _sessionMap;

  //NOTE : callback not thread safe, because http_parser holds raw pointer.
  http_parser_settings _parserSetting; 
};

