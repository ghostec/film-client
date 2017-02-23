#ifndef CLIENT_MESSAGEH
#define CLIENT_MESSAGEH

#include <string>
#include <uv.h>

namespace film { namespace client {

struct Message {
  uv_stream_t* handle;
  std::string data;
};

} }

#endif
