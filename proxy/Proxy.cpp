
#include "LooperPool.hpp"
#include "MultiplexLooper.hpp"
#include "TcpProxy.hpp"
#include "Daemon.hpp"
#include "UdpEndpoint.hpp"
#include "Logger.hpp"


static Daemon* gDaemon;
static TcpProxy* gTcpProxy;

// class LJProxy : public TcpProxy {
//   LJProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expired) :
//       TcpProxy(loopers, lport, expired),
//   {
    
//   }
//   void heartbeatHandler() {
    
//   }
// };

void onTerminate(int signo) {
  FOGW("\nServer terminate , signo=%d", signo);
  gTcpProxy->stopWork();
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  shared_ptr<LooperPool<MultiplexLooper> > spLoopers(new LooperPool<MultiplexLooper>(5));
  
  gDaemon = new Daemon();
  gTcpProxy = new TcpProxy(spLoopers, 3002, 150 * 1000);
  gDaemon->init(onTerminate);

  UdpEndpoint udpChannel(spLoopers->getLooper(), 3305, 1500);
  udpChannel.attach();
  
  gTcpProxy->startWork();
  gDaemon->startWork();
  return 0;
}

