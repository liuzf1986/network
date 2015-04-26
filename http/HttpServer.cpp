

#include "HttpServer.hpp"
#include "VecBuffer.hpp"
#include "Utils.hpp"

const SpVecData HttpServer::_400_VecData = HttpServer::genResponse_400();
const SpVecData HttpServer::_401_VecData = HttpServer::genResponse_401();
const SpVecData HttpServer::_403_VecData = HttpServer::genResponse_403();
const SpVecData HttpServer::_404_VecData = HttpServer::genResponse_404();

HttpServer::HttpServer(uint16_t port, SpLooperPool loopPool, uint32_t keepAliveMS, uint32_t idleTimeoutMS) :
    _keepAliveMS(keepAliveMS),
    _idleTimeoutMS(idleTimeoutMS),
    _server(port, loopPool),
    _timer(loopPool->getLooper(), TimerInterval, _keepAliveMS / TimerInterval),
    _sessionMap()
{
  _server.setConnectionHandler(std::bind(&HttpServer::onNewConnection, this, placeholders::_1));
}

void HttpServer::startWork() {
  _timer.attach();
  _server.startWork();
}

void HttpServer::stopWork() {
  _server.stopWork();
  _timer.detach();
}

void HttpServer::setMessageHandler(const HttpRequestHandler& handler) {
  _requestHandler = handler;
}

void HttpServer::setMessageHandler(HttpRequestHandler&& handler) {
  _requestHandler = std::move(handler);
}

SpVecData HttpServer::genResponse_400() {
  static const char* errRsp = "HTTP/1.1 400 Bad Request\r\n"
      "Content-Length: 10\r\n\r\n"
      "Failed\r\n\r\n";
  SpVecData spVecData = SpVecData(new VecData(strlen(errRsp) + 1));
  strcpy(reinterpret_cast<char*>(spVecData->data()), errRsp);
  return spVecData;
}

SpVecData HttpServer::genResponse_401() {
  static const char* errRsp = "HTTP/1.1 401 Unauthorized\r\n"
      "Content-Length: 10\r\n\r\n"
      "Failed\r\n\r\n";
  SpVecData spVecData = SpVecData(new VecData(strlen(errRsp) + 1));
  strcpy(reinterpret_cast<char*>(spVecData->data()), errRsp);
  return spVecData;  
}

SpVecData HttpServer::genResponse_403() {
  static const char* errRsp = "HTTP/1.1 403 Forbidden\r\n"
      "Content-Length: 10\r\n\r\n"
      "Failed\r\n\r\n";
  SpVecData spVecData = SpVecData(new VecData(strlen(errRsp) + 1));
  strcpy(reinterpret_cast<char*>(spVecData->data()), errRsp);
  return spVecData;  
}

SpVecData HttpServer::genResponse_404() {
  static const char* errRsp = "HTTP/1.1 404 Not Found\r\n"
      "Content-Length: 10\r\n\r\n"
      "Failed\r\n\r\n";
  SpVecData spVecData = SpVecData(new VecData(strlen(errRsp) + 1));
  strcpy(reinterpret_cast<char*>(spVecData->data()), errRsp);  
  return spVecData;  
}




