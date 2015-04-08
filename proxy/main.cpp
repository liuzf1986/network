

#include "Proxy.hpp"
#include "Daemon.hpp"

static Daemon* gDaemon;
static TcpProxy* gTcpProxy;

void onTerminate(int signo) {
  gTcpProxy->stopWork();
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  gDaemon = new Daemon();
  gTcpProxy = new TcpProxy(10, 3002, 1500);
  gDaemon->init(onTerminate);
  
  gTcpProxy->startWork();
  gDaemon->startWork();
  return 0;
}


















