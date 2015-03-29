
#include <stdint.h>
#include <map>
#include <atomic>

#include "TimeUtil.hpp"
#include "InetSock.hpp"
#include "MultiplexLooper.hpp"
#include "LooperPool.hpp"
#include "Dispatcher.hpp"
#include "FieldLenNetPack.hpp"
#include "PeerMessage.hpp"

#include "HashedWheelTimer.hpp"
#include "TimerWrap.hpp"

using namespace std;
using namespace netio;

class UserSession {
 public:
  UserSession(uint32_t uin, uint32_t sessKey) :
      _uin(uin),
      _sk(sessKey),      
      _tsCreate(TimeUtil::timestampMS()),
      _tsUpdate(_tsCreate),
      _seq(0)
  {}
  
  UserSession(uint32_t uin, uint32_t sessKey, uint32_t createTime) :
      _uin(uin),
      _sk(sessKey),
      _tsCreate(createTime),
      _tsUpdate(_tsCreate),
      _seq(0)
  {}
  
  void touch(uint64_t updateTime) {
    _tsUpdate = updateTime;
  }

  void touch() {
    _tsUpdate = TimeUtil::timestampMS();
  }

  uint32_t incSeq() {
    return _seq.fetch_add(1);
  }

  uint64_t lastUpdateTime() const {
    return _tsUpdate;
  }

  uint64_t createTime() const {
    return _tsCreate;
  }

  uint32_t sessionKey() const {
    return _sk;
  }

  uint32_t id() const {
    return _uin;
  }
  
 protected:
  uint32_t _uin;
  uint32_t _sk; // session key
  uint64_t _tsCreate;
  uint64_t _tsUpdate;
  atomic<uint32_t> _seq; // Sequence number just for generate request number.
};

// session type, like TcpUserSession, UdpUserSession
template <class STYPE>
class SessionMap {
  typedef unique_ptr<STYPE> UpSession;
 public:
  template <typename ... ARGS>
  void createSession(uint32_t uin, uint32_t sessKey, ARGS ... args) {
    _sessMap.insert(std::pair<uint32_t, UpSession>(uin, UpSession(new STYPE(uin, sessKey, args...))));
  }

  void dropSession(uint32_t uin) {
    _sessMap.erase(uin);
  }

  UpSession& getSession(uint32_t uin) {
    auto iter = _sessMap.find(uin);
    if(iter != _sessMap.end()) {
      return (*iter).second;
    }
  }
 private:
  map<uint32_t, UpSession> _sessMap;
};

// Netpack format codec @FieldLenNetPack
//template <typename NP>
class TcpUserSession : public UserSession {
  // message type by netpack
  //  typedef decltype(NP::readMessage(*(new SpVecBuffer(nullptr)))) MsgType;
  
 public:
  TcpUserSession(uint32_t uin, uint32_t sessKey, SpTcpConnection& connection, uint64_t createTime) :
      UserSession(uin, sessKey, createTime),
      _connection(connection)
  {}

  
  TcpUserSession(uint32_t uin, uint32_t sessKey, SpTcpConnection& connection) :
      UserSession(uin, sessKey),
      _connection(connection)
  {}

  /** 
   * 
   * 
   * 
   * @return 
   */
  ~TcpUserSession() {
    // if user session deconstruct, we don't interest connection event anymore.
    _connection->detach();
  }

  /** 
   * When sending request, message sequence number is generated by local session.
   */
  void sendRequest() {
    
  }
  
  /** 
   * Send packed message.
   * 
   * @param data : data that 
   */
  void send(const SpVecBuffer& data) {
    _connection->send(data);
  }

  /** 
   * Send multipul packed message
   * 
   * @param datas 
   */
  void sendMultiple(list<SpVecBuffer> &datas) {
    _connection->sendMultiple(datas);
  }
  
 private:
  SpTcpConnection _connection;
};

class UdpUserSession : public UserSession {
  
};


