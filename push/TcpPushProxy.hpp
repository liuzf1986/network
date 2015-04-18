#pragma once
/**
 * @file   TcpProxy.hpp
 * @author liuzf <liuzf@liuzf-H61M-DS2>
 * @date   Sun Apr 12 17:46:42 2015
 * 
 * @brief  Base tcp proxy for tcp server point, hold connection by TcpServer and managed by 
 *         SessionManager. 
 */

#include <memory>

#include "TcpServer.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "NetPackDispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "TcpConnection.hpp"
#include "Session.hpp"
#include "TimerWrap.hpp"
#include "HashedWheelTimer.hpp"
#include "Logger.hpp"
#include "GlobalCmdDef.hpp"
#include "common.pb.h"

using namespace netio;
using namespace std;

class TcpPushProxy {
 protected:
  enum { TimerInterval = 100 };
  
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;

  typedef FLNPack::MsgType::CmdType CmdType;  
  typedef NetPackDispatcher<FLNPack, TcpSource> TcpDispatcher;
  typedef Session<TcpSource> TcpSession;
  typedef shared_ptr<TcpSession> SpTcpSession;
  typedef shared_ptr<HashedWheelTimeout> SpWheelTimeout;    
 public:

  TcpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expireMS);
  TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expireMS);

  /** 
   * work controller
   */
  void startWork();
  void stopWork();

  /** 
   * register handlers for dispatcher
   * 
   * @param handler 
   */
  void registerHandler(const TcpDispatcher::Handler& handler);
  void registerHandler(TcpDispatcher::Handler&& handler);
  void registerHandler(CmdType& cmd, const TcpDispatcher::Handler& callback);
  void registerHandler(CmdType& cmd, TcpDispatcher::Handler&& callback);

 private:
  /** 
   * bind message callback with dispatcher when connnection established.
   * 
   * @param connection 
   */
  void onNewConnection(SpTcpConnection& connection) {
    LOGI(LOG_NETIO_TAG, "%s TcpPushProxy new connection", connection->strInfo());
    connection->attach();
  }

  /** 
   * adapter both TcpServer/UdpEndpoint for Dispatcher 
   * 
   * @param conn 
   * @param buffer 
   */
  void onNewMessage(SpTcpConnection conn, SpVecBuffer& buffer) {
    TcpSource source(conn);
    _dispatcher.dispatch(buffer, source);
  }

  /** 
   * update session idle time
   * 
   * @param spSession 
   */
  void touchSession(const SpTcpSession& spSession);

  /** 
   * idle session clean
   * 
   * @param spSession 
   */
  void removeSession(const SpTcpSession& spSession);

  /** 
   * receive data will trigger update session.
   * 
   * @param msg 
   * @param src 
   */
  void updateSession(const SpPeerMessage& msg, const TcpSource& src);
  
  SpLooperPool _loopPool;
  TcpServer _server;
 protected:
  // session and dispatcher is logical reference, it will real imply in subclass.
  SessionManager<TcpSource> _sm;
  TcpDispatcher _dispatcher;
  uint32_t _expireMS;  
  TimerWrap<HashedWheelTimer> _timer;
};


inline void TcpPushProxy::updateSession(const SpPeerMessage& msg, const TcpSource& src) {
  if(COMM_PKT_PING_REQUEST == msg->_cmd) {
    account::PingRequest pingReq;
    bool ret = pingReq.ParseFromArray(msg->_buffer->readablePtr(), msg->_buffer->readableSize());
    if(true == ret) {
      uint64_t cid = TcpSession::genConnectId(src);
      SpTcpSession spSession = _sm.findSessionByCid(cid);

      if(nullptr != spSession) {
        touchSession(spSession);
      } else {
        spSession = SpTcpSession(new TcpSession(pingReq.basereq().accid(), pingReq.basereq().seskey(), src));
        _sm.addSession(spSession);
        touchSession(spSession);
      }
    }
  }
}

inline void TcpPushProxy::removeSession(const SpTcpSession& spSession) {
  _server.removeConnection(spSession->getSource().getConnection());
  _sm.removeSession(spSession);
}

inline void TcpPushProxy::touchSession(const SpTcpSession& spSession) {
  auto rmfunc = std::bind(&TcpPushProxy::removeSession, this, spSession);
  SpWheelTimeout timeoutPtr = _timer.addTimeout(rmfunc, _expireMS);
  spSession->resetTimeout(timeoutPtr);
  spSession->touch();    
}










