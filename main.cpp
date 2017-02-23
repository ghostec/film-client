#include "client/client.h"

int main(int argc, char **argv) {
  film::client::Client client;
  client.connect();
}
