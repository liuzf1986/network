#pragma once

#include <memory>
#include <map>

#include "TcpServer.hpp"
#include "TcpConnection.hpp"
#include "HashedWheelTimer.hpp"
#include "TimerWrap.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "VecBuffer.hpp"

using namespace netio;

class HttpServer {
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLoopPool;
 public:
  
 private:
  SpLoopPool _loopPool;  
  uint32_t _keepAliveMS;
  TcpServer _server;
  TimerWrap<HashedWheelTimer> _timer;
};


