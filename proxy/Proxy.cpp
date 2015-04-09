#include <functional>

#include "Proxy.hpp"

using namespace netio;
using namespace std;

TcpProxy::TcpProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expired) :
    _loopPool(loopers),
    _server(lport, _loopPool), 
    _dispatcher(),
    _sm(),
    _timer(_loopPool->getLooper(), 100, expired / TimerInterval)
{
  _server.setNewConnectionHandler(std::bind(&TcpProxy::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpProxy::TcpProxy(size_t threadCount, uint16_t lport, uint32_t expired) :
    _loopPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(lport, _loopPool),
    _dispatcher(),
    _sm(),
    _timer(_loopPool->getLooper(), 100, expired / TimerInterval)  
{
  _server.setNewConnectionHandler(std::bind(&TcpProxy::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpProxy::startWork() {
  //   timerWrapper.addTimeout(func, 5000);
  _server.startWork();
}

void TcpProxy::stopWork() {
  _server.stopWork();
}


