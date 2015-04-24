#pragma once

#include <memory>

#include "PushProxy.hpp"
#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "TcpConnection.hpp"

class TcpPushProxy : public PushProxy<FLNPack, TcpSource>
{
  typedef typename PushProxy<FLNPack, TcpSource>::SessionMgrType SessionMgrType;
  typedef SessionMgrType::SessionType SessionType;
  typedef SessionMgrType::SpSession SpSession;
 public:
  TcpPushProxy(const SpLoopPool& loopers, uint16_t lport, uint32_t expireMS) :
      PushProxy<FLNPack, TcpSource>(loopers, lport, expireMS)
  {
    setupTcp();
  }
  TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expireMS) :
      PushProxy<FLNPack, TcpSource>(threadCount, lport, expireMS)
  {
    setupTcp();
  }
  
 private:
  /**
   * setup for tcp
   */
  void setupTcp() {
    _server.setConnectionHandler(std::bind(&TcpPushProxy::onConnectionOpen, this, std::placeholders::_1));
    _sm.setSessionRemoveHandler(std::bind(&TcpPushProxy::onSessionRemove, this, placeholders::_1));
  }

  /**
   * TcpServer new connection handler
   */
  void onConnectionOpen(const SpTcpConnection& connection) {
    LOGI("tpp", "%s new connection", connection->strInfo());
    connection->setNewMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
    connection->setCloseHandler(std::bind(&TcpPushProxy::onConnectionClose, this, placeholders::_1, placeholders::_2));    
    connection->attach();
  }

  /**
   * TcpServer session closed by peer handler
   */
  void onConnectionClose(const SpTcpConnection& connection, int cause) {
    uint64_t cid = SessionMgrType::SessionType::genConnectId(TcpSource(connection));
    SpSession spSession = _sm.findSessionByCid(cid);

    if(nullptr != spSession) {
      spSession->cancelTimeout();
      _sm.removeSession(spSession);
    }  
  }

  /** 
   * adapter both TcpServer/UdpEndpoint for Dispatcher, TcpServer receive data handler
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
  void onSessionRemove(const SpSession& spSession) {
    _server.removeConnection(spSession->getSource().getConnection());
  }
};

