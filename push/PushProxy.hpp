#pragma once

#include <memory>

#include "Dispatcher.hpp"
#include "TcpServer.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "Session.hpp"
#include "NetPackDispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "Logger.hpp"
#include "GlobalCmdDef.hpp"
#include "common.pb.h"

using namespace netio;
using namespace std;

template <class SRCType>
struct ServType {};

template <>
struct ServType<TcpSource> {
  typedef TcpServer Type;
};

template <>
struct ServType<UdpSource> {
  typedef UdpEndpoint Type;
};

template <typename NPType, typename SRCType>
class PushProxy {
 public:
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLoopPool;
  typedef NetPackDispatcher<NPType, SRCType> PushDispatcher;
  typedef typename NPType::MsgType::CmdType CmdType;
  typedef SessionManager<SRCType> SessionMgrType;
  
  PushProxy(const SpLoopPool& loopers, uint16_t lport, uint32_t expireMS) :
      _loopPool(loopers),
      _server(lport, _loopPool),
      _sm(_loopPool->getLooper(), expireMS),
      _dispatcher()
  {
    setup();
  }
  PushProxy(size_t threadCount, uint16_t lport, uint32_t expireMS) :
      _loopPool(new LooperPool<MultiplexLooper>(threadCount)),
      _server(lport, _loopPool),
      _sm(_loopPool->getLooper(), expireMS),
      _dispatcher()
  {
    setup();
  }


  /** 
   * proxy start work
   */
  void startWork() {
    _server.startWork();
    _sm.startWork();
  }

  /**
   * proxy stop work
   */
  void stopWork() {
    _sm.stopWork();
    _server.stopWork();
  }

  /** 
   * register handlers for dispatcher
   * 
   * @param handler 
   */
  void registerHandler(const typename PushDispatcher::Handler& handler) {
    _dispatcher.registerHandler(handler);
  }
  void registerHandler(typename PushDispatcher::Handler&& handler) {
    _dispatcher.registerHandler(std::move(handler));
  }
  void registerHandler(CmdType& cmd, const typename PushDispatcher::Handler& handler) {
    _dispatcher.registerHandler(cmd, handler);
  }
  void registerHandler(CmdType& cmd, typename PushDispatcher::Handler&& handler) {
    _dispatcher.registerHandler(cmd, std::move(handler));
  }

  
  void processHeartbeatReq(const SpPeerMessage& msg, const SRCType& src) {
    if(COMM_PKT_PING_REQUEST == msg->_cmd) {
      account::PingRequest pingReq;
      bool ret = pingReq.ParseFromArray(msg->_buffer->readablePtr(), msg->_buffer->readableSize());

      if(ret) {
        LOGI("pp", "%s heart beat uin=%d sk=%d seq=%d", src.getPeerInfo(),
             pingReq.basereq().accid(),
             pingReq.basereq().seskey(),
             msg->_seq);

        _sm.handlerHeartbeatReq(src, pingReq.basereq().accid(),  pingReq.basereq().seskey(), msg->_seq);
      }
    }
  }
  
 protected:
  void setup() {
    _dispatcher.registerHandler(std::bind(&PushProxy::processHeartbeatReq, this, placeholders::_1, placeholders::_2));
  }
  
  SpLoopPool _loopPool;
  typename ServType<SRCType>::Type _server;
  SessionMgrType _sm;
  PushDispatcher _dispatcher;
};



