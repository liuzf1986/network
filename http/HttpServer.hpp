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

class HttpServer {
  enum { TimerInterval = 100 };    
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef shared_ptr<HttpSession> SpSession;
  typedef std::map<int, SpSession> SessionMap;
  typedef shared_ptr<HashedWheelTimeout> SpTimeout;
  // return true if need to keep alive
  typedef function<bool(HttpSession&, const HttpMessageHolder&)> HttpRequestHandler;
 public:
  HttpServer(uint16_t port, SpLooperPool loopPool, uint32_t keepAliveMS, uint32_t idleTimeoutMS);

  // work control
  void startWork();
  void stopWork();

  void setMessageHandler(const HttpRequestHandler& handler);
  void setMessageHandler(HttpRequestHandler&& handler);
  
  void removeSession(MultiplexLooper* looper, int sid) {
    looper->postRunnablePopStack(std::bind(&HttpServer::removeSessionInternal, this, sid));
  }
  
 private:
  void onHttpMessage(HttpSession& session, const HttpMessageHolder& holder) {
    if(LIKELY(nullptr != _requestHandler)) {
      
      if(_requestHandler(session, holder)) {
        session.resetTimeout(_timer.addTimeout(std::bind(&HttpServer::removeSessionInternal, this, session.genSessionId()), _keepAliveMS));
      } else {
        removeSession(session.getLooper(), session.genSessionId());
      }
    } else {
      // send 403 Forbidden
      session.getConnection()->send(SpVecBuffer(new VecBuffer(_403_VecData, 0, _403_VecData->size())));
      removeSession(session.getLooper(), session.genSessionId());
    }
  }
  
  void onHttpError(HttpSession& session, int parserErrno) {
    LOGW("hts", "%s parsed failed, str=%s", session.getConnection()->strInfo(), http_errno_name(session.holder().getErrno()));
    session.getConnection()->send(SpVecBuffer(new VecBuffer(_400_VecData, 0, _400_VecData->size())));
    removeSession(session.getLooper(), session.genSessionId());
  }  
  
  /** 
   * New connection trigger by tcpsrever, create and save new session.
   * 
   * @param connection 
   */
  void onNewConnection(SpTcpConnection& connection) {
    SpSession spSession = SpSession(new HttpSession(connection));
    _sessionMap.insert(std::pair<int, SpSession>(spSession->genSessionId(), spSession));
    SpTimeout timeout = _timer.addTimeout(std::bind(&HttpServer::removeSessionInternal, this, spSession->genSessionId()), _idleTimeoutMS);
    spSession->resetTimeout(timeout);
    spSession->setMessageHandler(std::bind(&HttpServer::onHttpMessage, this, placeholders::_1, placeholders::_2));
    spSession->setErrorHandler(std::bind(&HttpServer::onHttpError, this, placeholders::_1, placeholders::_2));
    spSession->active();
  }

  /** 
   * remove session by session id
   * 
   * @param sid 
   */
  void removeSessionInternal(int sid) {
    auto iter = _sessionMap.find(sid);
    if(iter != _sessionMap.end()) {
      iter->second->deactive();
      iter->second->resetTimeout(nullptr);
      _server.removeConnection(iter->second->getConnection());
      _sessionMap.erase(iter);
    }
  }

  uint32_t _keepAliveMS;
  uint32_t _idleTimeoutMS;
  TcpServer _server;
  TimerWrap<HashedWheelTimer> _timer;
  SessionMap _sessionMap;
  
  HttpRequestHandler _requestHandler;

  // inner response
  static const SpVecData _400_VecData;  // Bad Request  
  static const SpVecData _401_VecData;  // Unauthorized
  static const SpVecData _403_VecData;  // Forbidden
  static const SpVecData _404_VecData;  // Not found

  static SpVecData genResponse_400();
  static SpVecData genResponse_401();
  static SpVecData genResponse_403();
  static SpVecData genResponse_404();
};











