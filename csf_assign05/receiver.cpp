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
  
  conn.connect(server_hostname, server_port);
  
  if (!conn.is_open()) {
    std::cerr << "not connected" << std::endl;
    return 1;
  }



  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  
  // Try to login
  Message message = Message(TAG_RLOGIN, username);
  if (!conn.send(message)) {
    std::cerr << "cannot login" << std::endl;
    return 1;
  }
  if (!conn.receive(message)) { //if server returns err for rlogin, print error payload to cerr
    return 1;
  }

  message = Message(TAG_JOIN, room_name);
  if (!conn.send(message)) {
    return 1;
  }
  if (!conn.receive(message)) { //if server returns err for join, print error payload to cerr
    return 1;
  }


  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  //
  
  
  bool continueLoop = true; //should i check if (message.tag == TAG_DELIVERY) 
  std::string roomReceiver;
  std::string sender;
  std::string userMessage;
  size_t colon1;
  size_t colon2;
  while (continueLoop) {
    continueLoop = conn.receive(message);
    if (!continueLoop) {
      continue;
    }
    
    colon1 = message.data.find(':');
    
    colon2 = message.data.find(':', colon1 + 1);

    roomReceiver = message.data.substr(0, colon1);
    
    sender = message.data.substr(colon1+1, colon2-colon1 - 1);
    userMessage = message.data.substr(colon2 + 1);

    if (roomReceiver != room_name) {
      //std::cout << roomReceiver << std::endl;
      //std::cout << room_name << std::endl;
      continue;
    }

    std::cout << sender << ": " << userMessage;


  }
  return 0;
}