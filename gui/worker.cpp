#include <iostream>
#include <QImage>
#include "worker.h"

namespace film { namespace gui {

void Worker::update_image(client::Message message) {
  std::cout << message.length << std::endl;
  auto image = QImage::fromData((const unsigned char*) message.data, message.length, "JPEG");
  auto scaled = image.scaled(800, 600,
      Qt::KeepAspectRatio, Qt::SmoothTransformation);
  auto pixmap_item = new QGraphicsPixmapItem(QPixmap::fromImage(scaled));

  emit set_pixmap_item(pixmap_item);
}

} }

#include "moc_worker.cpp"
