
#include "LooperPool.hpp"
#include "MultiplexLooper.hpp"
#include "TcpProxy.hpp"
#include "Daemon.hpp"
#include "UdpEndpoint.hpp"
#include "Logger.hpp"
#include "NetPackDispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "TcpConnection.hpp"

using namespace netio;

class TcpPushProxy : public TcpProxy {
 public:
  TcpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expired) :
      TcpProxy(loopers, lport, expired)
  {
    registerHandler(std::bind(&TcpPushProxy::sessionHandler, this,
                              std::placeholders::_1,
                              std::placeholders::_2));
  }
  
  TcpPushProxy(size_t threadCount, uint16_t lport, uint32_t expired) :
      TcpProxy(threadCount, lport, expired)
  {
    registerHandler(std::bind(&TcpPushProxy::sessionHandler, this,
                              std::placeholders::_1,
                              std::placeholders::_2));    
  }
 private:
  void sessionHandler(SpPeerMessage& msg, TcpSource& src) {
    COGI(" ============ ");
    COGI("msg proto=%d versoin=%d seq=%d, cmd=%d, content=%s", msg->_proto, msg->_version, msg->_seq, msg->_cmd, msg->_buffer->readablePtr());
    COGI(" ============ ");
  }
};


static Daemon* gDaemon;
static TcpPushProxy* gTcpProxy;

void onTerminate(int signo) {
  FOGW("\nServer terminate , signo=%d", signo);
  gTcpProxy->stopWork();
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  shared_ptr<LooperPool<MultiplexLooper> > spLoopers(new LooperPool<MultiplexLooper>(5));
  
  gDaemon = new Daemon();
  gTcpProxy = new TcpPushProxy(spLoopers, 3002, 150 * 1000);
  gDaemon->init(onTerminate);

  UdpEndpoint udpChannel(spLoopers->getLooper(), 3305, 1500);
  udpChannel.attach();
  
  gTcpProxy->startWork();
  gDaemon->startWork();
  return 0;
}











