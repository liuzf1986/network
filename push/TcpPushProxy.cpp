#include "TcpPushProxy.hpp"

using namespace netio;
using namespace std;


TcpPushProxy::TcpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expireMS) :
    _loopPool(loopers),
    _server(lport, _loopPool),
    _sm(),
    _dispatcher(),
    _expireMS(expireMS),
    _timer(_loopPool->getLooper(), TimerInterval, expireMS / TimerInterval)
{
  _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
  _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
  _dispatcher.registerHandler(std::bind(&TcpPushProxy::updateSession, this, placeholders::_1, placeholders::_2));
}
  
TcpPushProxy::TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expireMS) :
    _loopPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(lport, _loopPool),
    _sm(),
    _dispatcher(),
    _expireMS(expireMS),
    _timer(_loopPool->getLooper(), TimerInterval, expireMS / TimerInterval)      
{
  _server.setConnectionHandler(std::bind(&TcpPushProxy::onNewConnection, this, std::placeholders::_1));
  _server.setMessageHandler(std::bind(&TcpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2));
  _dispatcher.registerHandler(std::bind(&TcpPushProxy::updateSession, this, placeholders::_1, placeholders::_2));    
}

void TcpPushProxy::startWork() {
  _server.startWork();
  _timer.attach();
}
  
void TcpPushProxy::stopWork() {
  _server.stopWork();
  _timer.detach();
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
