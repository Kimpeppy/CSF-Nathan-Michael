#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  int fd = open_clientfd(argv[1], argv[2]);
  if (fd < 0) {
    Message error = Message(TAG_ERR, "cannot login");
    return 1;
  }
  // TODO: send slogin message
  Message slogin = Message(TAG_SLOGIN, username);
  Message ok = Message(TAG_OK, "server login success");
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate



  return 0;
}


