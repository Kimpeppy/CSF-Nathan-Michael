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
  Connection connection;
  connection.connect(server_hostname, server_port);
  if(!connection.is_open()) {
    // TODO: Throw error here and exit code
    std::cerr << "connection not open " << server_hostname << std::endl;
    return 1;
  }
  
  // TODO: send slogin message
  Message slogin = Message(TAG_SLOGIN, username);
  if (!connection.send(slogin)) {
    std::cerr << "could not send in login message" << std::endl;
    return 1;
  }
  Message message;
  if (!connection.receive(message)) {
    return 1;
  }
  if (message.tag == TAG_ERR) {
    std::cerr << message.data << std::endl;
    return 1;
  }

  
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string input;
  std::string party;
  bool exitTrue = false;

  while(getline(cin, input)) {
    if (input.find("/join") != std::string::npos) {
      party = input.substr(6, input.length());
      message = Message(TAG_JOIN, party);

    } 
    else if (input.find("/leave") != std::string::npos) {
      message = Message(TAG_LEAVE, "leaving room");
    }
    else if (input.find("/quit") != std::string::npos) {
      message = Message(TAG_QUIT, "quitting room");
      exitTrue = true;
    }
    else if (input.find("/") != std::string::npos) {
      std::cerr << "bad command" << std::endl;
      continue;
    }
    else {
      message = Message(TAG_SENDALL, input);
    }
    if (!connection.send(message)) {
      exitTrue = false;
    }
    if (!connection.receive(message)) {
      exitTrue = false;
    }
    if (exitTrue) {
      break;
    }

  }
  


  return 0;
}




