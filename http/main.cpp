#include "LooperPool.hpp"
#include "MultiplexLooper.hpp"
#include "Daemon.hpp"
#include "Logger.hpp"
#include "TcpConnection.hpp"
#include "HttpServer.hpp"

using namespace netio;

static Daemon* gDaemon;
static HttpServer* gHttpServer;


void onTerminate(int signo) {
  FOGW("\nServer terminate , signo=%d", signo);
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  g_inner_logger.setLogLevel(LEVEL_INFO);  
  shared_ptr<LooperPool<MultiplexLooper> > spLoopers(new LooperPool<MultiplexLooper>(5));

  gHttpServer = new HttpServer(8088, spLoopers, 60 * 1000, 10 * 1000);
  gHttpServer->startWork();
  
  gDaemon = new Daemon();
  gDaemon->init(onTerminate);
  gDaemon->startWork();
  return 0;
}











