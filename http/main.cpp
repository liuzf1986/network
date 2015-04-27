#include "LooperPool.hpp"
#include "MultiplexLooper.hpp"
#include "Daemon.hpp"
#include "Logger.hpp"
#include "TcpConnection.hpp"
#include "HttpServer.hpp"
#include "HttpSession.hpp"
#include "HttpMessage.hpp"
#include "ResponseHelper.hpp"

using namespace netio;

static Daemon* gDaemon;
static HttpServer* gHttpServer;



bool test_handler(HttpSession& session, const HttpMessageHolder& holder) {
#define DBG_URL(XX)                            \
  if(holder.hasUrl_##XX()) {                   \
    ScatterString ss = holder.getUrl_##XX();   \
    string str(ss._addr, ss._length);          \
    LOGI("test", #XX" %s", str.c_str());       \
  }

  DBG_URL(SCHEMA);
  DBG_URL(HOST);
  DBG_URL(PORT);
  DBG_URL(PATH);
  DBG_URL(QUERY);
  DBG_URL(FRAGMENT);
  DBG_URL(USERINFO);
#undef DBG_URL
  const char* hello = "hello";
  session.getConnection()->send(ResponseHelper::createResponse_200(strlen(hello), hello));
  return false;
}


void onTerminate(int signo) {
  FOGW("\nServer terminate , signo=%d", signo);
  gDaemon->stopWork();
}

int main(int argc, char *argv[])
{
  //  g_inner_logger.setLogLevel(LEVEL_INFO);  
  shared_ptr<LooperPool<MultiplexLooper> > spLoopers(new LooperPool<MultiplexLooper>(5));

  gHttpServer = new HttpServer(8088, spLoopers, 60 * 1000, 10 * 1000);
  gHttpServer->setMessageHandler(test_handler);
  gHttpServer->startWork();
  
  gDaemon = new Daemon();
  gDaemon->init(onTerminate);
  gDaemon->startWork();
  return 0;
}











