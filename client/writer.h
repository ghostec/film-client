#ifndef CLIENT_WRITERH
#define CLIENT_WRITERH

#include <mutex>
#include "message.h"

namespace film { namespace client {

class Writer {
public:
  Writer();
  ~Writer();
  void write(Message message);
private:
  std::mutex write_mutex;
};

} } 

#endif
