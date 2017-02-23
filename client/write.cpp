#include <uv.h>
#include <mutex>
#include <cstring>
#include "write.h"

namespace film { namespace client {

void write(Message message) {
  static std::mutex mutex;

  uv_buf_t* buf = new uv_buf_t();
  buf->len = strlen(message.data);
  buf->base = new char[buf->len];
  memcpy(buf->base, message.data, buf->len);

  uv_write_t* req = new uv_write_t();
  req->handle = message.handle;

  struct BufMutex { uv_buf_t* buf; std::mutex* mutex; };
  req->data = (void*) new BufMutex({ .buf = buf, .mutex = &mutex });

  mutex.lock();
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
