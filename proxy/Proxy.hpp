#pragma once

#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "TcpConnection.hpp"
#include "TcpServer.hpp"


// This class is proxy that manage tcp read write, by uin number.

using namespace netio;
using namespace std;

// NP is netpack protocol.
template <class NP>
class Proxy {
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef shared_ptr<TcpConnection> SpTcpConnection;
  typedef decltype(NP::readMessage(*(new SpVecBuffer(nullptr)))) MsgType;
  typedef function<void(SpTcpConnection, MsgType&)> OnNewMessage;
 public:
  // constructors 
  Proxy (uint16_t port, SpLooperPool loopPool);
  Proxy (uint16_t port, int threadCount);

  // start/stop wrok
  void startWork();
  void stopWork();

  
 private:
  // callback for TcpServer when new connection comes.
  void onNewConnection(int, SpTcpConnection&);
  // callback for TcpServer when tcpconnection receive a new message.
  void onNewData(SpTcpConnection&, SpVecBuffer&);
  
  SpLooperPool _looperPool;
  TcpServer _server;
};

template <class NP>
inline void Proxy<NP>::onNewConnection(int key, SpTcpConnection& connection) {
  
}

template <class NP>
inline void Proxy<NP>::onNewData(SpTcpConnection& connection, SpVecBuffer& buffer) {
  
}

template <class NP>
Proxy<NP>::Proxy(uint16_t port, SpLooperPool loopPool) :
    _looperPool(loopPool),
    _server(port, _looperPool)
{
  _server.setNewConnectionHandler(std::bind(&Proxy::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
  _server.setNewMessageHandler(std::bind(&Proxy::onNewData, this, std::placeholders::_1, std::placeholders::_2));
}

template <class NP>
Proxy<NP>::Proxy(uint16_t port, int threadCount) :
    _looperPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(port, _looperPool)
{}

template <class NP>
void Proxy<NP>::startWork() {
  _server.startWork();
}

template <class NP>
void Proxy<NP>::stopWork() {
  _server.stopWork();
}


















