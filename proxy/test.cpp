
#include <stdio.h>
#include <time.h>


#include "Proxy.hpp"
#include "InetSock.hpp"
#include "TcpProxy.hpp"

#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"


int main(int argc, char *argv[])
{
  shared_ptr<LooperPool<MultiplexLooper> > loopers(new LooperPool<MultiplexLooper>(5));
  TcpProxy<FieldLenNetpack<GenericLenFieldHeader> > proxy(loopers, static_cast<uint16_t>(3002), static_cast<uint16_t>(8550));

  sleep(10);

  return 0;
}


