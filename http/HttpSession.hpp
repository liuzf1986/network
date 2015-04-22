
#include "http_parser.h"
#include "TcpConnection.hpp"

using namespace netio;

class HttpSession {
 public:
  HttpSession(const SpTcpConnection& connection) :
      _spConn(connection)
  {
  }

  /** 
   * generate session id for http session.
   * 
   * @return 
   */
  uint64_t genSessionId() const {
    uint64_t cid = 0L;
    cid = _spConn->getPeerIp();
    return ((cid << 32) | _spConn->getPeerPort());
  }

  
 private:
  SpTcpConnection _spConn;
  http_parser _parser;
  http_parser_settings _parserSetting;
};


