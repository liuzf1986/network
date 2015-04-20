#pragma once
#include <memory>

#include "UdpEndpoint.hpp"
#include "Session.hpp"
#include "HashedWheelTimer.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "FieldLenNetPack.hpp"
#include "Dispatcher.hpp"


using namespace netio;
using namespace std;

class UdpPushProxy {
  enum { TimerInterval = 100 };
  
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef FLNPack::MsgType::CmdType CmdType;  
  typedef NetPackDispatcher<FLNPack, UdpSource> UdpDispatcher;
  typedef shared_ptr<Session<TcpSource> > SpSession;
 public:
  UdpPushProxy(const SpLooperPool& loopers, uint16_t lport, uint32_t expireMS) :
      _loopPool(loopers),
      _spUdpEpt(new UdpEndpoint(_loopPool->getLooper(), lport, 1500)),
      _sm(loopers->getLooper(), expireMS),
      _dispatcher()
  {
    _spUdpEpt->setNewMessageHandler(std::bind(&UdpPushProxy::onNewMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
    _dispatcher.registerHandler(std::bind(&UdpPushProxy::updateSession, this, placeholders::_1, placeholders::_2));
  }

  void startWork() {
    _spUdpEpt->attach();
    _sm.startWork();
  }
  
  void stopWork() {
    _sm.stopWork();
    _spUdpEpt->attach();
  }

  void registerHandler(const UdpDispatcher::Handler& handler) {
    _dispatcher.registerHandler(handler);
  }

  void registerHandler(UdpDispatcher::Handler&& handler) {
    _dispatcher.registerHandler(std::move(handler));
  }

  void registerHandler(CmdType& cmd, const UdpDispatcher::Handler& callback) {
    _dispatcher.registerHandler(cmd, callback);
  }
  
  void registerHandler(CmdType& cmd, UdpDispatcher::Handler&& callback) {
    _dispatcher.registerHandler(cmd, std::move(callback));
  }
  
 private:
  void onNewMessage(SpVecBuffer& buffer, uint32_t rip, uint16_t rport) {
    UdpSource source(_spUdpEpt, rip, rport);
    _dispatcher.dispatch(buffer, source);
  }

  void updateSession(const SpPeerMessage& msg, const UdpSource& src) {
  }
  
  SpLooperPool _loopPool;
  SpUdpEndpoint _spUdpEpt;

  SessionManager<Session<UdpSource> > _sm;
  UdpDispatcher _dispatcher;
};











