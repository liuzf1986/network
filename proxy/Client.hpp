#pragma once

#include "TcpConnection.hpp"
#include "TcpConnector.hpp"
#include "MultiplexLooper.hpp"
#include "Dispatcher.hpp"
#include "NetPackDispatcher.hpp"
#include "FieldLenNetPack.hpp"

using namespace netio;

class TcpClient {
  typedef NetPackDispatcher<FLNPack, TcpConnection> TcpDispatcher;
 public:
  TcpClient(const char* rip, uint16_t rport, uint16_t lport);
  
  void startWork();
  void stopWork();
 private:
  void onNewConnection(int fd, const InetAddr& addr);
  
  MultiplexLooper _looper;
  TcpConnector _connector;
  TcpDispatcher _dispatcher;
};



















