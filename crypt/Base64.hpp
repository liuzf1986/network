#pragma once
#include <unistd.h>

class Base64Codec {
  // private:
  /*private calls, we have ensure enough space for output*/
  static size_t getEncodeBufferSize(const char* srcptr, size_t strlen);
  static size_t encode(char* dstptr, size_t dstlen, const char* srcptr, size_t strlen);
  static size_t getDecodeBufferSize(const char* srcptr, size_t strlen);
  static size_t decode(char* dstptr, size_t dstlen, const char* srcptr, size_t strlen);
};

















