#include "TcpClient.hpp"
#include "FieldLenNetPack.hpp"
#include "common.pb.h"
#include "GlobalCmdDef.hpp"

using namespace netio;



int main(int argc, char *argv[])
{
  TcpClient* tcpClient = new TcpClient("127.0.0.1", 3002);
  tcpClient->startWork();

  sleep(2);
  
  account::PingRequest pingReq;
  account::BaseReq baseReq;

  baseReq.set_accid(32);
  baseReq.set_seskey(100);

  pingReq.set_allocated_basereq(&baseReq);

  int contentLen = pingReq.ByteSize();
  SpVecBuffer buffer = FLNPack::createNetPackBuffer(static_cast<uint16_t>(contentLen), FLNPProto_PROTOBUF, 1, 10, COMM_PKT_PING_REQUEST);
  pingReq.SerializeToArray(buffer->writtablePtr(), buffer->writtableSize());
  buffer->markWrite(contentLen);
  
  SpTcpConnection connection = tcpClient->getConnection();
  //  FLNPack::createNetPackBuffer(uint16_t conlen, uint8_t proto, uint8_t version, uint16_t seq, uint32_t cmd)
  
  // SpVecBuffer buffer = FLNPack::writeMessage(FLNPProto_PROTOBUF, 1, 0, 100,
  //                                            "hello", strlen("hello"));

  for(int i = 0; i < 3; i ++) {
    connection->send(buffer);
    sleep(6);
  }


  sleep(10000);
  tcpClient->stopWork();
  
  return 0;
}














