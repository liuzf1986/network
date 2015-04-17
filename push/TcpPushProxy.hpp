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

  TcpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expired) :
      _loopPool(loopers),
      _server(lport, _loopPool),
      _sm(_loopPool->getLooper(), expired),
      _dispatcher()      
  {
    _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
    _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
    _dispatcher.registerHandler(std::bind(&TcpPushProxy::updateSession, this, placeholders::_1, placeholders::_2));
  }
  
  TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expired) :
      _loopPool(new LooperPool<MultiplexLooper>(threadCount)),
      _server(lport, _loopPool),
      _sm(_loopPool->getLooper(), expired),
      _dispatcher()      
  {
    _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
    _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
    _dispatcher.registerHandler(std::bind(&TcpPushProxy::updateSession, this, placeholders::_1, placeholders::_2));    
  }

  void startWork() {
    _server.startWork();
    _sm.enableIdleKick();
  }
  
  void stopWork() {
    _server.stopWork();
    _sm.disableIdleKick();
  }

  void registerHandler(const TcpDispatcher::Handler& handler) {
    _dispatcher.registerHandler(handler);
  }

  void registerHandler(TcpDispatcher::Handler&& handler) {
    _dispatcher.registerHandler(std::move(handler));
  }

  void registerHandler(CmdType& cmd, const TcpDispatcher::Handler& callback) {
    _dispatcher.registerHandler(cmd, callback);
  }
  
  void registerHandler(CmdType& cmd, TcpDispatcher::Handler&& callback) {
    _dispatcher.registerHandler(cmd, std::move(callback));
  }

 private:
  /** 
   * bind message callback with dispatcher when connnection established.
   * 
   * @param connection 
   */
  void onNewConnection(SpTcpConnection& connection) {
    FOGI("TcpPushProxy connection establish, remoteaddr=%s ", connection->getPeerAddr().strIpPort().c_str());
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

  void updateSession(const SpPeerMessage& msg, const TcpSource& src) {
    if(COMM_PKT_PING_REQUEST == msg->_cmd) {
      account::PingRequest pingReq;
      bool ret = pingReq.ParseFromArray(msg->_buffer->readablePtr(), msg->_buffer->readableSize());
      if(true == ret) {
        uint64_t cid = TcpSession::genConnectId(src);
        SpTcpSession spSession = _sm.findSessionByCid(cid);

        if(nullptr != spSession) {
          _sm.touchSession(spSession);
        } else {
          _sm.addSession(SpTcpSession(new TcpSession(pingReq.basereq().accid(), pingReq.basereq().seskey(), src)));
        }
      }
    }
  }
  
  SpLooperPool _loopPool;
  TcpServer _server;
 protected:
  // session and dispatcher is logical reference, it will real imply in subclass.
  SessionManager<TcpSource> _sm;
  TcpDispatcher _dispatcher;
};












