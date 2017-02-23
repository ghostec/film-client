#include <uv.h>
#include "writer.h"

namespace film { namespace client {

Writer::Writer() {}
Writer::~Writer() {}

void Writer::write(Message message) {
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

} }
