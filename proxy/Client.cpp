#include "TcpClient.hpp"

using namespace netio;



int main(int argc, char *argv[])
{
  TcpClient* tcpClient = new TcpClient("127.0.0.1", 3002);
  tcpClient->startWork();
  sleep(2);
  tcpClient->stopWork();
  delete tcpClient;

  sleep(1);
  
  return 0;
}











