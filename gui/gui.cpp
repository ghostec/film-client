#include <QImage>
#include "gui.h"

namespace film { namespace gui {

GUI::GUI() {
  auto image = QImage(800, 600, QImage::Format_Indexed8);
  image.fill(0);
  pixmap_item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
  scene = new QGraphicsScene();
  scene->addItem(pixmap_item);
  view = new QGraphicsView();
  view->setScene(scene);
  view->show();
}

GUI::~GUI() {}

void GUI::set_pixmap_item(QGraphicsPixmapItem* pixmap_item) {
  auto ref = this->pixmap_item;
  scene->removeItem(ref);
  delete ref;

  this->pixmap_item = pixmap_item;
  scene->addItem(pixmap_item);

  view->show();
}

} }

#include "moc_gui.cpp"
