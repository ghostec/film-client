#include <uv.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "server.h"

namespace film { namespace server {

Server::Server() {}
Server::~Server() {}

int Server::start() {
  auto loop = uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", 3000, &addr);

  uv_tcp_bind(&server, (const struct sockaddr*) &addr, 0);
  server.data = this;
  int r = uv_listen((uv_stream_t*) &server, SOMAXCONN, connection_cb);
  if (r) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    return 1;
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}

void Server::register_observer(Observer observer) {
  observers.push_back(observer);
}

void Server::write(Message message) {
  uv_buf_t* buf = new uv_buf_t();
  buf->len = message.data.size() + 1;
  buf->base = new char[buf->len];
  memcpy(buf->base, &message.data[0], buf->len);

  uv_write_t* req = new uv_write_t();
  req->handle = message.handle;

  struct BufMutex { uv_buf_t* buf; std::mutex* mutex; };
  req->data = (void*) new BufMutex({ .buf = buf, .mutex = &write_mutex });

  write_mutex.lock();
  uv_write(req, req->handle, buf, 1,
    [](uv_write_t* req, int status) -> void {
      if(req && req->data) {
        auto helper = (BufMutex*) req->data;
        helper->mutex->unlock();
        delete helper->buf;
        delete helper;
      }
      if(req) delete req;
  });
}

// Private members

void Server::notify_observers(Message message) {
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

void Server::read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
  if (nread <= 0) return;

  std::string message_data;
  std::istringstream iss(buf->base);
  while (std::getline(iss, message_data, '\r')) {
    if (message_data.empty()) continue;
    ((Server*)handle->data)->notify_observers({ handle, message_data });
  }
}

void Server::alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

void Server::connection_cb(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    abort();
  }

  auto loop = uv_default_loop();
  uv_tcp_t* client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t*) client) == 0) {
    client->data = server->data;
    uv_read_start((uv_stream_t*) client, alloc_cb, read_cb);
  }
  else {
    uv_close((uv_handle_t*) client, NULL);
  }
}

} }
