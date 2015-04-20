#include "TcpPushProxy.hpp"

using namespace netio;
using namespace std;


TcpPushProxy::TcpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expireMS) :
    _loopPool(loopers),
    _server(lport, _loopPool),
    _sm(_loopPool->getLooper(), expireMS),
    _dispatcher()
{
  _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
  _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
  _server.setConnectionCloseHandler(std::bind(&TcpPushProxy::onResetConnection, this, placeholders::_1, placeholders::_2));
  _dispatcher.registerHandler(std::bind(&TcpPushProxy::processHeartbeatReq, this, placeholders::_1, placeholders::_2));
  _sm.setSessionRemoveHandler(std::bind(&TcpPushProxy::onSessionRemove, this, placeholders::_1));
}
  
TcpPushProxy::TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expireMS) :
    _loopPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(lport, _loopPool),
    _sm(_loopPool->getLooper(), expireMS),
    _dispatcher()
{
  _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
  _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
  _server.setConnectionCloseHandler(std::bind(&TcpPushProxy::onResetConnection, this, placeholders::_1, placeholders::_2));
  _dispatcher.registerHandler(std::bind(&TcpPushProxy::processHeartbeatReq, this, placeholders::_1, placeholders::_2)); 
  _sm.setSessionRemoveHandler(std::bind(&TcpPushProxy::onSessionRemove, this, placeholders::_1)); 
}

void TcpPushProxy::startWork() {
  _server.startWork();
  _sm.startWork();
}
  
void TcpPushProxy::stopWork() {
  _sm.stopWork();
  _server.stopWork();
}

void TcpPushProxy::registerHandler(const TcpDispatcher::Handler& handler) {
  _dispatcher.registerHandler(handler);
}

void TcpPushProxy::registerHandler(TcpDispatcher::Handler&& handler) {
  _dispatcher.registerHandler(std::move(handler));
}

void TcpPushProxy::registerHandler(CmdType& cmd, const TcpDispatcher::Handler& callback) {
  _dispatcher.registerHandler(cmd, callback);
}
  
void TcpPushProxy::registerHandler(CmdType& cmd, TcpDispatcher::Handler&& callback) {
  _dispatcher.registerHandler(cmd, std::move(callback));
}
