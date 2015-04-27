#pragma once

#include <string.h>

#include "VecBuffer.hpp"

using namespace netio;

/**
 * @file   ResponseHelper.hpp
 * @author liuzf <liuzf@liuzf-H61M-DS2>
 * @date   Mon Apr 27 01:08:05 2015
 * 
 * @brief  helper for construct VecBuffer.
 * 
 * 
 */


class ResponseHelper {
 public:
  static SpVecBuffer createResponse_200(size_t bodylen);
  static SpVecBuffer createResponse_200(size_t bodylen, const char* body);
};



















