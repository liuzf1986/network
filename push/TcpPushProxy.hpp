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
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef FLNPack::MsgType::CmdType CmdType;  
  typedef NetPackDispatcher<FLNPack, TcpSource> TcpDispatcher;
  typedef Session<TcpSource> TcpSession;
  typedef shared_ptr<TcpSession> SpTcpSession;

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
    LOGI("tpp", "%s new connection", connection->strInfo());
    connection->attach();
  }

  /** 
   * handler by conenction reset by peer
   * 
   * @param connection 
   */
  void onResetConnection(const SpTcpConnection& connection, int cause) {
    uint64_t cid = TcpSession::genConnectId(TcpSource(connection));
    SpTcpSession spSession = _sm.findSessionByCid(cid);

    if(nullptr != spSession) {
      spSession->cancelTimeout();
      _sm.removeSession(spSession);
    }
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
   * idle session clean
   * 
   * @param spSession 
   */
  void onSessionRemove(const SpTcpSession& spSession);

  /** 
   * receive data will trigger update session.
   * 
   * @param msg 
   * @param src 
   */
  void processHeartbeatReq(const SpPeerMessage& msg, const TcpSource& src);
  
  SpLooperPool _loopPool;
  TcpServer _server;
 protected:
  // session and dispatcher is logical reference, it will real imply in subclass.
  SessionManager<TcpSource> _sm;
  TcpDispatcher _dispatcher;
};


inline void TcpPushProxy::processHeartbeatReq(const SpPeerMessage& msg, const TcpSource& src) {
  if(COMM_PKT_PING_REQUEST == msg->_cmd) {
    account::PingRequest pingReq;
    bool ret = pingReq.ParseFromArray(msg->_buffer->readablePtr(), msg->_buffer->readableSize());

    if(true == ret) {
      LOGI("tpp", "%s heart beat uin=%d, sk=%d, seq=%d", src.getPeerInfo(), pingReq.basereq().accid(), pingReq.basereq().seskey(), msg->_seq);       
      _sm.handlerHeartbeatReq(src, pingReq.basereq().accid(),  pingReq.basereq().seskey(), msg->_seq);
    }
  }
}

inline void TcpPushProxy::onSessionRemove(const SpTcpSession& spSession) {
  _server.removeConnection(spSession->getSource().getConnection());
}

