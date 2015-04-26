#pragma once
#include <map>
#include <stdint.h>

#include "http_parser.h"

using namespace std;

enum class ParseState {
  ParseHeader,
  ParseBody,
  ParseDone
};

struct ScatterString {
  ScatterString() :
      _addr(nullptr),
      _length(0)
  {}
  ScatterString(const char* addr, size_t length) :
      _addr(addr),
      _length(length)
  {}

  bool operator< (const ScatterString& rhs) const
  {
    if(_length == rhs._length)
      return strncasecmp(_addr, rhs._addr, _length) < 0;
    return _length < rhs._length;
  }  

  void clear() {
    _addr = nullptr;
    _length = 0;
  }
  
  const char* _addr;
  size_t _length;
};



class Compare {
  const ScatterString& _field;
 public:
  Compare ( const ScatterString& field )
      : _field(field)
  {}
  bool operator() ( const std::pair<ScatterString, ScatterString>& item )
  {
    if(item.first._length == _field._length) {
      if(0 == strncasecmp(item.first._addr, _field._addr, _field._length)) {
        return true;
      }
    }
    return false;
  }    
};

/**
 * TcpConnection handleRead function guarantee SpVecBuffer will not
 * be free if we have not call markRead. So we can just save pointer and 
 * length for http data.
 * 
 * This struct will hold datas parsed from callback.
 * This holder work unthreadsafe.
 * We must comsume it before SpVecBuffer::markRead
 */
class HttpMessageHolder {
 public:
  HttpMessageHolder(void* userdata, enum http_parser_type parserType) {
    _parser.data = userdata;
    _parserType = parserType;
  }

  void reset() {
    http_parser_init(&_parser, _parserType);
    _url.clear();
    _body.clear();
    _hangField.clear();
    _fieldMap.clear();
  }
  
  int setUrl(const char* at, size_t length) {
    _url._addr = at;
    _url._length = length;

    return http_parser_parse_url(_url._addr, _url._length, 0, &_parserUrl);
  }

  void setHeaderField(const char* at, size_t length) {
    _hangField._addr = at;
    _hangField._length = length;
  }

  void setHeaderValue(const char* at, size_t length) {
    _fieldMap.insert(std::pair<ScatterString, ScatterString>(_hangField, ScatterString(at, length)));
  }

  void setBody(const char* at, size_t length) {
    _body._addr = at;
    _body._length = length;
  }

  bool hasField(const ScatterString& field) const {
    const map<ScatterString, ScatterString>::const_iterator iter =
        find_if(_fieldMap.begin(), _fieldMap.end(), Compare(field));
    if(iter != _fieldMap.end()) {
      if(LIKELY(iter->second._length > 0)) {
        return true;
      }
    }
  }

  const std::string getField(const std::string& field) const {
    const map<ScatterString, ScatterString>::const_iterator iter =
        find_if(_fieldMap.begin(), _fieldMap.end(), Compare(ScatterString(field.c_str(), field.length())));
    if(iter != _fieldMap.end() && 0 != iter->second._length) {
      return std::string(iter->second._addr, iter->second._length);
    }
    return std::string();
  }

  ScatterString getField(const ScatterString& field) const {
    const map<ScatterString, ScatterString>::const_iterator iter =
        find_if(_fieldMap.begin(), _fieldMap.end(), Compare(field));
    if(iter != _fieldMap.end()) {
      return iter->second;
    }
    return ScatterString(nullptr, 0);
  }

  const ScatterString& getBody() const {
    return _body;
  }

  int major_version () const {
    return _parser.http_major;
  }


  int minor_version () const {
    return _parser.http_minor;
  }

  bool keepAlive () const {
    return (0 != http_should_keep_alive(&_parser));
  }

  void setState(ParseState state) {
    _state = state;
  }

  bool isHeaderDone() {
    if(ParseState::ParseBody == _state || ParseState::ParseDone == _state) {
      return true;
    }
    return false;
  }

  bool isMessageDone() {
    if(ParseState::ParseDone == _state) {
      return true;
    }
    return false;
  }

  http_errno getErrno() const {
    return static_cast<http_errno>(_parser.http_errno);
  }

  size_t consume(const http_parser_settings* setting, const char* data, size_t len) {
    return http_parser_execute(&_parser, setting, data, len);
  }

#define  URL_HAS(XX) bool hasUrl_##XX() const {      \
    return _parserUrl.field_set & (1 << UF_##XX);   \
  }
  URL_HAS(SCHEMA)
  URL_HAS(HOST)
  URL_HAS(PORT)
  URL_HAS(PATH)
  URL_HAS(QUERY)
  URL_HAS(FRAGMENT)
  URL_HAS(USERINFO)
#undef URL_HAS

#define URL_GET(XX) ScatterString getUrl_##XX() const {  \
  if(_parserUrl.field_data[UF_##XX].len > 0) {           \
  return ScatterString(_url._addr +                      \
  _parserUrl.field_data[UF_##XX].off,                    \
  _parserUrl.field_data[UF_##XX].len);                   \
  }                                                      \
  return ScatterString(nullptr, 0);                      \
  }

  URL_GET(SCHEMA)
  URL_GET(HOST)
  URL_GET(PORT)
  URL_GET(PATH)
  URL_GET(QUERY)
  URL_GET(FRAGMENT)
  URL_GET(USERINFO)
#undef URL_GET
  

 private:
  ParseState _state;
  
  // header
  ScatterString _url;
  http_parser_url _parserUrl;

  // parser
  http_parser _parser;
  enum http_parser_type _parserType;

  // http body
  ScatterString _body;

  // header attributes
  ScatterString _hangField;  // hang http header field temperary
  map<ScatterString, ScatterString> _fieldMap;
};



