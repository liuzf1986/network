
#include "Proxy.hpp"

Proxy::Proxy(uint16_t port, SpLooperPool loopPool) :
    _looperPool(loopPool),
    _server(port, _looperPool)
{
  _server.setNewConnectionHandler(std::bind(&Proxy::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
  _server.setNewMessageHandler(std::bind(&Proxy::onNewData, this, std::placeholders::_1, std::placeholders::_2));
}

Proxy::Proxy(uint16_t port, int threadCount) :
    _looperPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(port, _looperPool)
{}

void Proxy::startWork() {
  _server.startWork();
}

void Proxy::stopWork() {
  _server.stopWork();
}


