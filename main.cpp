#include <QApplication>
#include <iostream>
#include "network/constants.h"
#include "film-network/write.h"
#include "film-network/client.h"
#include "gui/gui.h"
#include "gui/worker.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  film::gui::GUI gui;
  film::gui::Worker worker;

  film::network::Client client;

  client.set_inner_connection_cb([](uv_stream_t* handle) {
    film::network::write({
      .handle = handle,
      .data = film::network::REGISTER_CLIENT_MESSAGE,
      .length = strlen(film::network::REGISTER_CLIENT_MESSAGE) + 1
    });
  });

  QObject::connect(&worker, &film::gui::Worker::set_pixmap_item,
      &gui, &film::gui::GUI::set_pixmap_item, Qt::QueuedConnection);

  client.register_observer(
    std::bind(&film::gui::Worker::update_image, &worker, std::placeholders::_1)
  );

  client.start("0.0.0.0", 3001);

  return app.exec();
}
