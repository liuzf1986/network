#include "HttpServer.hpp"
#include "Utils.hpp"



int message_begin_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->updateParseState(ParseState::ParseHeader);
  return 0;
}

int url_callback(http_parser* parser, const char *at, size_t length) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  if(sessionPtr->onUrl(at, length)) {
    
  }
  
  char temp[100] = {0};
  strncpy(temp, at, length);
  LOGI("hp", "%s ,%s", __func__,temp);
  return 0;
}

int status_callback(http_parser* parser, const char *at, size_t length) {
  char temp[100] = {0};
  strncpy(temp, at, length);
  LOGI("hp", "%s ,%s", __func__,temp);  
  return 0;  
}

int header_field_callback(http_parser* parser, const char *at, size_t length) {
  char temp[100] = {0};
  strncpy(temp, at, length);
  LOGI("hp", "%s ,%s", __func__,temp);  
  return 0;  
}

int header_value_callback(http_parser* parser, const char *at, size_t length) {
  char temp[100] = {0};
  strncpy(temp, at, length);
  LOGI("hp", "%s ,%s", __func__,temp);  
  return 0;  
}

int headers_complete_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  ASSERT(nullptr != sessionPtr);
  sessionPtr->updateParseState(ParseState::ParseBody);
  return 0;  
}

int body_callback(http_parser* parser, const char *at, size_t length) {
  char temp[100] = {0};
  strncpy(temp, at, length);
  LOGI("hp", "%s ,%s", __func__,temp);  
  return 0;  
}

int test_count = 0;
const char* test_rsp = "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Content-Length: 13\r\n"
    "Proxy-Connection: close\r\n"
    "Date: Thu, 31 Dec 2009 20:55:48 +0000\r\n"
    "\r\n"
    "hello world";

int message_complete_callback(http_parser* parser) {
  HttpSession* sessionPtr = static_cast<HttpSession*>(parser->data);
  SpVecBuffer buffer = SpVecBuffer(new VecBuffer(200));
  sprintf((char*)buffer->writtablePtr(), "%s %d", test_rsp, test_count);
  
  buffer->markWrite(strlen((char*)buffer->writtablePtr()));

  ++test_count;
  
  ASSERT(nullptr != sessionPtr);
  sessionPtr->updateParseState(ParseState::ParseDone);
  sessionPtr->getConnection()->send(buffer);
  return 0;  
}














