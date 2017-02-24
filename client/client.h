#ifndef CLIENT_CLIENTH
#define CLIENT_CLIENTH

#include <functional>
#include <uv.h>
#include "message.h"
#include "observable.h"

namespace film { namespace client {

typedef std::function<void(Message message)> Observer;

class Client : public Observable<Observer, Message> {
public:
  Client();
  ~Client();
  int connect();
private:
  virtual void notify_observers(Message message);
  static void delete_buf_cb(uv_handle_t* handle, void* ptr);
  static void msg_read_cb(uv_stream_t *handle, void *msg, int size);
  static void read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
  static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
  static void connection_cb(uv_connect_t* connection, int status);
  static void write_cb(uv_write_t* req, int status);
  static void close_cb(uv_handle_t* handle);
};

} }

#endif
