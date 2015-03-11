
#include "Proxy.hpp"

Proxy::Proxy(uint16_t port, int threadCount) :
    _looperPool(new LooperPool<MultiplexLooper>(threadCount)),
    _server(port, _looperPool)
{
  
}
















