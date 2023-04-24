#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }
  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
    std::cerr << "not connected" << std::endl;
    return 1;
  }
  Message message = Message(TAG_RLOGIN, username);
  if(!conn.communicate(message)) {
    return 1;
  }
  message = Message(TAG_JOIN, room_name);
  if (!conn.communicate(message)) {
    return 1;
  }
  conn.receiver_loop(message, room_name);
  return 0;
}