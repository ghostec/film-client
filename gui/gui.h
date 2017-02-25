#ifndef GUI_GUIH
#define GUI_GUIH

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace film { namespace gui {

class GUI : public QObject {
  Q_OBJECT
public:
  GUI();
  ~GUI();
public slots:
  void set_pixmap_item(QGraphicsPixmapItem* pixmap_item);
private:
  QGraphicsPixmapItem* pixmap_item;
  QGraphicsScene* scene;
  QGraphicsView* view;
};

} }

#endif
