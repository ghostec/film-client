#include <QApplication>
#include <thread>
#include "client/client.h"
#include "gui/gui.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  auto gui = film::gui::GUI();
  gui.show();

  film::client::Client client;

  client.register_observer(
    std::bind(&film::gui::GUI::update_image, &gui, std::placeholders::_1)
  );

  std::thread t_client(&film::client::Client::connect, &client);

  return app.exec();
}
