#ifndef GUI_GUIH
#define GUI_GUIH

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "client/message.h"

namespace film { namespace gui {

class GUI {
public:
  GUI();
  ~GUI();
  void show();
  void update_image(client::Message message);
private:
  QGraphicsPixmapItem* pixmap_item;
  QGraphicsScene* scene;
  QGraphicsView* view;
};

} }

#endif
