#include <QApplication>
#include <thread>
#include "client/client.h"
#include "gui/gui.h"

int main(int argc, char **argv) {
  film::client::Client client;
  std::thread t_client(&film::client::Client::connect, &client);

  QApplication app(argc, argv);
  film::gui::GUI().show();
  return app.exec();
}
