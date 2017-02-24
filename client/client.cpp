#include <uv_msg_framing.h>
#include <string>
#include <sstream>
#include <iostream>
#include "client.h"
#include "write.h"
#include "message.h"

namespace film { namespace client {

Client::Client() {}
Client::~Client() {}

int Client::connect() {
  auto loop = uv_default_loop();

  uv_msg_t socket;
  uv_msg_init(loop, &socket, UV_TCP);
  uv_tcp_keepalive((uv_tcp_t*) &socket, 1, 60);

  sockaddr_in dest;
  uv_ip4_addr("0.0.0.0", 3000, &dest);

  uv_connect_t connect;
  connect.data = this;
  uv_tcp_connect(&connect, (uv_tcp_t*) &socket, (const struct sockaddr*) &dest, connection_cb);

  return uv_run(loop, UV_RUN_DEFAULT);
}

void Client::notify_observers(Message message) {
  auto loop = uv_default_loop();

  struct ObserverMessage { Observer observer; Message message; };

  for(const auto& observer : observers) {
    uv_work_t* req = new uv_work_t();

    req->data = (void*) new ObserverMessage {
      .observer = observer,
      .message = message
    };

    uv_queue_work(loop, req,
      [](uv_work_t* req) -> void {
        auto om = (ObserverMessage*) req->data;
        om->observer(om->message);
      },
      [](uv_work_t* req, int status) -> void {
        delete (ObserverMessage*) req->data;
        delete req;
    });
  }
}

void Client::delete_buf_cb(uv_handle_t* handle, void* ptr) {
   delete ptr;
}

void Client::msg_read_cb(uv_stream_t *handle, void *msg, int size) {
  if (size <= 0) return;

  printf("new message here (%d bytes): %s\n", size, (char*)msg);
  auto data = new char[size + 1];
  memcpy(data, (char*) msg, size);
  ((Client*)handle->data)->notify_observers({
    .handle = handle, .data = (char*) msg, .length = (size_t) size
  });
}

void Client::connection_cb(uv_connect_t* connection, int status)
{
  printf("connected.\n");

  uv_stream_t* stream = connection->handle;
  stream->data = connection->data;

  write({ .handle = stream, .data = "this is a test", .length = 15 });
  uv_msg_read_start((uv_msg_t*) stream, alloc_cb, msg_read_cb, delete_buf_cb);
}

void Client::alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  *buf = uv_buf_init(new char[suggested_size], suggested_size);
}

void Client::close_cb(uv_handle_t* handle)
{
  printf("closed.");
}

} }
