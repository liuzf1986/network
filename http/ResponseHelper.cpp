#include "ResponseHelper.hpp"

#include "VecBuffer.hpp"

static const char* const RSP_OK_PATTERN =
    "HTTP/1.1 200 OK\r\n"                         
    "Connection: close\r\n"                       
    "Content-Length: %lu\r\n"                      
    "\r\n";

static constexpr size_t getPatternLen() {
  return strlen(RSP_OK_PATTERN) - strlen("%lu");
}

static size_t uintegerLengthIf(size_t number) {
  if (number < 10) return 1;
  if (number < 100) return 2;
  if (number < 1000) return 3;
  if (number < 10000) return 4;
  if (number < 100000) return 5;
  if (number < 1000000) return 6;
  if (number < 10000000) return 7;
  if (number < 100000000) return 8;
  if (number < 1000000000) return 9;
  return 10;
}

SpVecBuffer ResponseHelper::createResponse_200(size_t bodylen) {
  size_t headerLen = getPatternLen() + uintegerLengthIf(bodylen);
  SpVecBuffer spBuf(new VecBuffer(headerLen + bodylen));
  sprintf(spBuf->writtablePtr(), RSP_OK_PATTERN, bodylen);
  spBuf->markWrite(headerLen);
  return spBuf;
}

SpVecBuffer ResponseHelper::createResponse_200(size_t bodylen, const char* body) {
  size_t headerLen = getPatternLen() + uintegerLengthIf(bodylen);
  SpVecBuffer spBuf(new VecBuffer(headerLen + bodylen));
  sprintf(spBuf->writtablePtr(), RSP_OK_PATTERN, bodylen);
  spBuf->markWrite(headerLen);
  memcpy(spBuf->writtablePtr(), body, bodylen);
  spBuf->markWrite(bodylen);
  return spBuf;
}


