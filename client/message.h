#ifndef CLIENT_MESSAGEH
#define CLIENT_MESSAGEH

#include <uv.h>

namespace film { namespace client {

struct Message {
  uv_stream_t* handle;
  const char* data;
};

} }

#endif
