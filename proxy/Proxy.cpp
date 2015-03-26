
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include "InetSock.hpp"
//#include "TcpProxy.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"

#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"

#include "HashedWheelTimer.hpp"
#include "TimerWrap.hpp"

#include "UserSession.hpp"

using namespace netio;


void foo() {
  COGFUNC();
}

int main(int argc, char *argv[])
{
  HashedWheelTimer timer(100, 5);

  function<void()> func = foo;

  //  SpHashedWheelTimeout timeout = timer.addTimeout(func, 2000);

  for(int i = 0; i < 100; i++) {
    //  COGI("tick i = %d", i);
    //timer.tick();
  }

  SessionMap<TcpUserSession> sessMap;

  SpTcpConnection spConn;

  sessMap.createSession(200, 1, spConn);

  shared_ptr<LooperPool<MultiplexLooper> > loopers(new LooperPool<MultiplexLooper>(5));
  //  TcpProxy<FieldLenNetpack<GenericLenFieldHeader> > proxy(loopers, static_cast<uint16_t>(3002), static_cast<uint16_t>(8550));
  TimerWrap<HashedWheelTimer> timerWrapper(loopers->getLooper(), 100, 5);
  timerWrapper.attach();

  TcpDispatcher<FieldLenNetpack<GenericLenFieldHeader>> dispatcher(loopers, static_cast<uint16_t>(3002));
  dispatcher.startWork();

  struct timeval tv;
  gettimeofday(&tv, NULL);

  
  
  timerWrapper.addTimeout(func, 5000); 
  
  

  sleep(10);
  timerWrapper.detach();
  
  return 0;
}










