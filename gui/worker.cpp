#include <iostream>
#include <QImage>
#include <string>
#include "network/constants.h"
#include "worker.h"

namespace film { namespace gui {

void Worker::update_image(network::Message message) {
  std::string msg(message.data);

  if(msg != network::FRAME_MESSAGE) return;

  auto image = QImage::fromData((const unsigned char*) message.data + msg.size() + 1, message.length - msg.size() - 1, "JPEG");
  auto scaled = image.scaled(800, 600,
      Qt::KeepAspectRatio, Qt::SmoothTransformation);
  auto pixmap_item = new QGraphicsPixmapItem(QPixmap::fromImage(scaled));

  emit set_pixmap_item(pixmap_item);
}

} }

#include "moc_worker.cpp"
