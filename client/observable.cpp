#include <uv.h>
#include "observable.h"

namespace film { namespace client {

Observable::Observable() {}
Observable::~Observable() {}

void Observable::register_observer(Observer observer) {
  observers.push_back(observer);
}

void Observable::notify_observers(Message message) {
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

} }
