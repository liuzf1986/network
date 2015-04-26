#pragma once


/**
 * @file   ResponseHelper.hpp
 * @author liuzf <liuzf@liuzf-H61M-DS2>
 * @date   Mon Apr 27 01:08:05 2015
 * 
 * @brief  helper for construct VecBuffer.
 * 
 * 
 */
#define RSP_OK_PATTERN                          \
  "HTTP/1.1 200 OK\r\n"                         \
  "Connection: close\r\n"                       \
  "Content-Length: %d\r\n"                      \
  "\r\n"
  

class ResponseHelper {
  
};
