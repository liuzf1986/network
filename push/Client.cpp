#include "TcpClient.hpp"
#include "FieldLenNetPack.hpp"

using namespace netio;



int main(int argc, char *argv[])
{
  TcpClient* tcpClient = new TcpClient("127.0.0.1", 3002);
  tcpClient->startWork();

  sleep(2);

  
  
  SpTcpConnection connection = tcpClient->getConnection();
  SpVecBuffer buffer = FLNPack::writeMessage(FLNPProto_PROTOBUF, 1, 0, 100,
                                             "hello", strlen("hello"));
  connection->send(buffer);

  sleep(10000);
  tcpClient->stopWork();
  
  return 0;
}














