#include "client.h"
#include "write.h"
#include "message.h"

namespace film { namespace client {

Client::Client() {}
Client::~Client() {}

int Client::connect() {
  auto loop = uv_default_loop();

  uv_tcp_t socket;
  uv_tcp_init(loop, &socket);
  uv_tcp_keepalive(&socket, 1, 60);

  sockaddr_in dest;
  uv_ip4_addr("0.0.0.0", 3000, &dest);

  uv_connect_t connect;
  uv_tcp_connect(&connect, &socket, (const struct sockaddr*) &dest, connection_cb);

  return uv_run(loop, UV_RUN_DEFAULT);
}

void Client::connection_cb(uv_connect_t* connection, int status)
{
  printf("connected.\n");

  uv_stream_t* stream = connection->handle;

  uv_buf_t buffer[] = {
    {.base = "hello", .len = 5},
    {.base = "world", .len = 5}
  };

  write({ .handle = stream, .data = "this is a test" });
  uv_read_start(stream, alloc_cb, read_cb);
}

void Client::read_cb(uv_stream_t* tcp, ssize_t nread, const uv_buf_t* buf)
{
  if(nread >= 0) {
    //printf("read: %s\n", tcp->data);
    printf("read: %s\n", buf->base);
  }
  else {
    //we got an EOF
    uv_close((uv_handle_t*)tcp, close_cb);
  }

  //cargo-culted
  delete buf->base;
  //delete buf;
}

void Client::alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  *buf = uv_buf_init(new char[suggested_size], suggested_size);
}

void Client::close_cb(uv_handle_t* handle)
{
  printf("closed.");
}

void Client::write_cb(uv_write_t* req, int status)
{
  if (status) {
    fprintf(stderr, "uv_write error: %s\n", uv_strerror(status));
    return;
  }
  printf("wrote.\n");
  //uv_close((uv_handle_t*)req->handle, on_close);
}


} }
