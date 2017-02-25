#include <QApplication>
#include <thread>
#include "film-network/client.h"
#include "gui/gui.h"
#include "gui/worker.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  film::gui::GUI gui;
  film::gui::Worker worker;

  film::network::Client client;

  QObject::connect(&worker, &film::gui::Worker::set_pixmap_item,
      &gui, &film::gui::GUI::set_pixmap_item, Qt::QueuedConnection);

  client.register_observer(
    std::bind(&film::gui::Worker::update_image, &worker, std::placeholders::_1)
  );

  std::thread t_client(&film::network::Client::start, &client);

  return app.exec();
}
