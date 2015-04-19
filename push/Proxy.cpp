
#include "LooperPool.hpp"
#include "MultiplexLooper.hpp"
#include "TcpPushProxy.hpp"
#include "UdpPushProxy.hpp"
#include "Daemon.hpp"
#include "UdpEndpoint.hpp"
#include "Logger.hpp"
#include "NetPackDispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "TcpConnection.hpp"

using namespace netio;

static Daemon* gDaemon;
static TcpPushProxy* gTcpProxy;
static UdpPushProxy* gUdpProxy;

void onTerminate(int signo) {
  FOGW("\nServer terminate , signo=%d", signo);
  gTcpProxy->stopWork();
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  g_inner_logger.setLogLevel(LEVEL_INFO);  
  shared_ptr<LooperPool<MultiplexLooper> > spLoopers(new LooperPool<MultiplexLooper>(5));
  gDaemon = new Daemon();
  gTcpProxy = new TcpPushProxy(spLoopers, 3002, 10 * 1000);
  gDaemon->init(onTerminate);

  UdpEndpoint udpChannel(spLoopers->getLooper(), 3305, 1500);
  udpChannel.attach();
  
  gTcpProxy->startWork();
  gDaemon->startWork();
  return 0;
}

