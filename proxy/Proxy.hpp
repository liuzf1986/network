#pragma once

#include <map>
#include <mutex>

#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "TcpConnection.hpp"
#include "TcpServer.hpp"
#include "FieldLenNetPack.hpp"


/** This class is proxy that manage tcp read write, by uin number.
 *  the codec proto is GenFieldLenPack.
 */

using namespace netio;
using namespace std;

class Proxy {
  typedef shared_ptr<LooperPool<MultiplexLooper> > SpLooperPool;
  typedef shared_ptr<TcpConnection> SpTcpConnection;
  typedef function<void(SpTcpConnection, SpPeerMessage&)> OnNewMessage;
 public:
  // constructors 
  explicit Proxy (uint16_t port, SpLooperPool loopPool);
  explicit Proxy (uint16_t port, int threadCount);

  // start/stop wrok
  void startWork();
  void stopWork();

  void registerObserver(uint32_t cmd, OnNewMessage& callback);
 private:
  // callback for TcpServer when new connection comes.
  void onNewConnection(int, SpTcpConnection&);
  // callback for TcpServer when tcpconnection receive a new message.
  void onNewData(SpTcpConnection&, SpVecBuffer&);
  
  SpLooperPool _looperPool;
  TcpServer _server;

  map<uint32_t, OnNewMessage> _dispatchMap;
  mutable mutex _mapMutex;
};

inline void Proxy::onNewConnection(int key, SpTcpConnection& connection) {
  
}

inline void Proxy::onNewData(SpTcpConnection& connection, SpVecBuffer& buffer) {
  SpPeerMessage message = GenFieldLenPack::readMessage(buffer);

  int _predMsgLen = 100;

  // buffer not contain complete message
  if(nullptr == message) {
    if((0 == buffer->readableSize()) && (buffer->writtableSize() < _predMsgLen)) {
      buffer.reset(new VecBuffer(_predMsgLen));
    } else {
      ssize_t expect = GenFieldLenPack::peekMessageLen(buffer);
      if(expect < 0) {
        expect = _predMsgLen;
      }

      buffer->ensure(expect);
    }
  } else {
    // if we have parse and generate a new message, dispatch it.
    map<uint32_t, OnNewMessage>::const_iterator iter = _dispatchMap.find(message->_info._cmd);
    if(iter != _dispatchMap.end()) {
      (*iter).second(connection, message);
    }
  }
}



