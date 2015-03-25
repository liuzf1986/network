
#include <stdio.h>
#include <time.h>


#include "InetSock.hpp"
#include "TcpProxy.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"

#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"

#include "HashedWheelTimer.hpp"

using namespace netio;

void foo() {
  COGFUNC();
}

int main(int argc, char *argv[])
{
  HashedWheelTimer timer(9, 100);

  function<void()> func = foo;

  SpHashedWheelTimeout timeout = timer.addTimeout(func, 300);

  COGI("11");  
  timer.tick();

  timeout->cancel();
  timeout = nullptr;

  COGI("22");
  timer.tick();

  COGI("33");  
  timer.tick();

  COGI("44");
  timer.tick();
  
  
  shared_ptr<LooperPool<MultiplexLooper> > loopers(new LooperPool<MultiplexLooper>(5));
  TcpProxy<FieldLenNetpack<GenericLenFieldHeader> > proxy(loopers, static_cast<uint16_t>(3002), static_cast<uint16_t>(8550));

  sleep(10);



  
  
  return 0;
}

















