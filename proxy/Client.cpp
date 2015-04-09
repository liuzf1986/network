
#include <functional>

#include "Client.hpp"
#include "Logger.hpp"

using namespace netio;
using namespace std;

TcpClient::TcpClient(const char* rip, uint16_t rport, uint16_t lport) :
    _looper(),
    _connector(&_looper, lport, InetAddr(rip, rport)),
    _dispatcher()
{}

void TcpClient::startWork() {
  _connector.setNewConnCallback(std::bind(&TcpClient::onNewConnection, this, std::placeholders::_1, std::placeholders::_2));
  _connector.attach();
  _looper.stopLoop();

  _connector.connect();
}

void TcpClient::stopWork() {
  _looper.stopLoop();
}

void TcpClient::onNewConnection(int fd, const netio::InetAddr &addr) {
  COGFUNC();
}

#if 0
using namespace netio;
using namespace std;

MultiplexLooper looper;
InetAddr remoteAddr("127.0.0.1", 3002);

void onNewConnection(int fd, const InetAddr& addr) {
  COGFUNC();
  SpTcpConnection spConn = SpTcpConnection(new TcpConnection(&looper, fd, addr.getSockAddr()));

  PMInfo msgInfo;
  msgInfo._cmd = 100;
  msgInfo._seq = 0;
  msgInfo._proto = PMPROTO_PROTOBUF;
  msgInfo._version = 1;

  SpVecBuffer buffer =  FieldLenNetpack<GenericLenFieldHeader>::createPackLayoutBuffer(msgInfo, strlen("hello"));
  memcpy(buffer->writtablePtr(), "hello", strlen("hello"));
  buffer->markWrite(strlen("hello"));

  spConn->send(buffer);
}

int main(int argc, char *argv[])
{
  thread thrd(std::bind(&MultiplexLooper::startLoop, &looper));
  
  TcpConnector connector(&looper, 8000, remoteAddr);
  
  connector.attach();
  connector.setNewConnCallback(std::bind(onNewConnection, std::placeholders::_1, std::placeholders::_2));
  connector.connect();

  sleep(1000);

  return 0;
}

#endif

