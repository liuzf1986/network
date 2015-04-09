
#include <functional>

#include "Client.hpp"
#include "Logger.hpp"

using namespace netio;
using namespace std;

TcpClient::TcpClient(const char* rip, uint16_t rport) :
    _thread(nullptr),
    _looper(),
    _connector(&_looper, 0, InetAddr(rip, rport)),
    _dispatcher()
{}

void TcpClient::startWork() {
  _thread = unique_ptr<thread>(new thread(std::bind(&MultiplexLooper::startLoop, &_looper)));
  
  _connector.setNewConnCallback(std::bind(&TcpClient::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
  _connector.attach();
  _connector.connect();
}

void TcpClient::stopWork() {
  _connector.detach();
  _connector.disconnect();
  _looper.stopLoop();
  _thread->join();
  
}

void TcpClient::onNewConnection(int fd, const netio::InetAddr &addr) {
  COGFUNC();
}











