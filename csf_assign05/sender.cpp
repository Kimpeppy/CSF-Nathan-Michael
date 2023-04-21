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
  Connection connection;
  connection.connect(server_hostname, server_port);
  if(!connection.is_open()) {
    std::cerr << "connection not open " << server_hostname << std::endl;
    return 1;
  }
  Message message = Message(TAG_SLOGIN, username);
  if (!connection.communicate(message)) {
    return 1;
  }
  std::string input;
  std::string party;
  bool exitTrue = false;

  connection.check_end_loop(message, input, party, exitTrue);
  return 0;
}




