
#include <stdio.h>
#include <time.h>


#include "InetSock.hpp"
#include "TcpProxy.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"

#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"

using namespace netio;

int main(int argc, char *argv[])
{
  shared_ptr<LooperPool<MultiplexLooper> > loopers(new LooperPool<MultiplexLooper>(5));
  //  TcpProxy<FieldLenNetpack<GenericLenFieldHeader> > proxy(loopers, static_cast<uint16_t>(3002), static_cast<uint16_t>(8550));


  TcpDispatcher<FieldLenNetpack<GenericLenFieldHeader>> dispatcher(loopers, static_cast<uint16_t>(3002));
  dispatcher.startWork();

  sleep(10);

  
  return 0;
}










