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
    std::cerr << message.data << std::endl;
    return 1;
  }

  message = Message(TAG_JOIN, room_name);
  if (!conn.send(message)) {
    std::cerr << "cannot join room" << std::endl;
    return 1;
  }
  if (!conn.receive(message)) { //if server returns err for join, print error payload to cerr
    std::cerr << message.data << std::endl;
    return 1;
  }
  std::cout << username << "joins" << room_name << std::endl;


  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  //
  
  bool continueLoop; //should i check if (message.tag == TAG_DELIVERY) 
  std::string roomReceiver;
  std::string sender;
  std::string userMessage;
  size_t colon1;
  size_t colon2;
  while (continueLoop == true) {
    continueLoop = conn.receive(message);
    if (!continueLoop) {
      std::cerr << "did not recieve message properly" << std::endl; 
      continue;
    }
    
    colon1 = message.data.find(':');
    colon2 = message.data.find(':', colon1 + 1);

    roomReceiver = message.data.substr(0, colon1);
    sender = message.data.substr(colon1, colon2);
    userMessage = message.data.substr(colon2, message.data.length());

    if (roomReceiver != room_name) {
      std::cerr << "did not receive from the right room" << std::endl;
      continue;
    }

    std::cout << sender << ": " << userMessage << std::endl;


  }
    return 0;
  }