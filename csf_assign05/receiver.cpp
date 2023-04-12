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


  // TODO: connect to server
  conn.m_fd = open_clientfd(server_hostname, server_port);
  conn = Connection(m_fd);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message m = Message(TAG_RLOGIN, "");
  if(conn.send(m)) {
    //do something
  }
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)


  return 0;
}
