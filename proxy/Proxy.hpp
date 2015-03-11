#pragma once

#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "TcpConnection.hpp"
#include "TcpServer.hpp"

using namespace netio;
using namespace std;

class Proxy {
 public:
  Proxy (uint16_t port, int threadCount);
 private:
  SpLooperPool _looperPool;
  TcpServer _server;
};

