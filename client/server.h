#ifndef SERVER_SERVERH
#define SERVER_SERVERH

#include <uv.h>
#include <vector>
#include <string>
#include <functional>
#include <mutex>

namespace film { namespace server {

struct Message {
  uv_stream_t* handle;
  std::string data;
};

typedef std::function<void(Message message)> Observer;

class Server {
public:
  Server();
  ~Server();
  int start();
  void write(Message message);
  void register_observer(Observer observer);
private:
  std::vector<Observer> observers;
  std::mutex write_mutex;

  void notify_observers(Message message);
  static void read_cb(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
  static void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
  static void connection_cb(uv_stream_t *server, int status);
};

} }

#endif
