#pragma once

#include <string>
#include <vector>

// ascii uri encode

class UriCodec {
  
 public:
  static std::string encode(const std::string& str);
  static std::string decode(const std::string& str);

  // private:
  /*private calls, we have ensure enough space for output*/
  static size_t getEncodeBufferSize(const char* srcptr, size_t strlen);
  static size_t encode(char* dstptr, size_t dstlen, const char* srcptr, size_t strlen);
  static size_t getDecodeBufferSize(const char* srcptr, size_t strlen);
  static size_t decode(char* dstptr, size_t dstlen, const char* srcptr, size_t strlen);
};












