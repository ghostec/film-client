#ifndef CLIENT_CLIENTH
#define CLIENT_CLIENTH

#include <uv.h>
#include "observable.h"

namespace film { namespace client {

class Client {
public:
  Observable observable;

  Client();
  ~Client();
  int connect();
private:
  static void read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
  static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
  static void connection_cb(uv_connect_t* connection, int status);
  static void write_cb(uv_write_t* req, int status);
  static void close_cb(uv_handle_t* handle);
};

} }

#endif