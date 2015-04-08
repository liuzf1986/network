#pragma once

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

using namespace netio;
using namespace std;

class TcpProxy {
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef NetPackDispatcher<FLNPack, TcpConnection> TcpDispatcher;
  enum { TimerInterval = 100 };
 public:
  TcpProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expired);
  TcpProxy(size_t threadCount, uint16_t lport, uint32_t expired);

  void startWork();
  void stopWork();
  
 private:
  void onNewConnection(int connId, SpTcpConnection& connection) {
    connection->setNewMessageHandler(std::bind(&TcpDispatcher::dispatch, &_dispatcher, std::placeholders::_1, std::placeholders::_2));
    connection->attach();
  }

  
  SpLooperPool _loopPool;
  TcpServer _server;
  TcpDispatcher _dispatcher;
  SessionManager _sm;
  TimerWrap<HashedWheelTimer> _timer;
};



















