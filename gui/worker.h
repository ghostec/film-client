#ifndef GUI_WORKERH
#define GUI_WORKERH

#include <QObject>
#include <QGraphicsPixmapItem>
#include "film-network/message.h"

namespace film { namespace gui {

class Worker : public QObject {
  Q_OBJECT
public:
  Worker() {}
  ~Worker() {}
  void update_image(network::Message message);
signals:
  void set_pixmap_item(QGraphicsPixmapItem* pixmap_item);
};

} }

#endif
