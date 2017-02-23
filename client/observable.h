#ifndef CLIENT_OBSERVABLEH
#define CLIENT_OBSERVABLEH

#include <vector>
#include <functional>
#include "message.h"

namespace film { namespace client {

typedef std::function<void(Message message)> Observer;

class Observable {
public:
  Observable();
  ~Observable();
  void register_observer(Observer observer);
  void notify_observers(Message message);
private:
  std::vector<Observer> observers;
};

} }

#endif
