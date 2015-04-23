#include "HttpServer.hpp"



int message_begin_callback(http_parser* parser) {
  
}

int url_callback(http_parser* parser, const char *at, size_t length) {
  
}

int status_callback(http_parser* parser, const char *at, size_t length) {
  
}

int header_field_callback(http_parser* parser, const char *at, size_t length) {
  
}

int header_value_callback(http_parser* parser, const char *at, size_t length) {
  
}

int headers_complete_callback(http_parser* parser) {
  
}

int body_callback(http_parser* parser, const char *at, size_t length) {
  
}

int message_complete_callback(http_parser* parser) {

}

