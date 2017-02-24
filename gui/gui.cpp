#include <QImage>
#include <cstring>
#include <iostream>
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
}

GUI::~GUI() {}

void GUI::update_image(client::Message message) {
  auto image = QImage::fromData((const unsigned char*) message.data, strlen(message.data));
  auto scaled = image.scaled(800, 600,
      Qt::KeepAspectRatio, Qt::SmoothTransformation);

  auto ref = pixmap_item;
  pixmap_item = new QGraphicsPixmapItem(QPixmap::fromImage(scaled));
  scene->removeItem(ref);
  delete ref;

  show();
}

void GUI::show() {
  scene->removeItem(pixmap_item);
  scene->addItem(pixmap_item);
  view->show();
}

} }
