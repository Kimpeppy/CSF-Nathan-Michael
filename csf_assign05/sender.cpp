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
  


  Connection conn;
  int fd = open_clientfd(argv[1], argv[2]);
  if (fd < 0) {
    Message error = Message(TAG_ERR, "cannot login");
    Rio_writen(fd, error.messageString(), error.payloadSize());
    //TODO: replace this with the corresponding way to return the function
    return 1;
  }
  conn.connect(server_hostname, server_port);
  if(!conn.is_open()) {
   // Throw error here

  }
  
  // TODO: send slogin message
  Message slogin = Message(TAG_SLOGIN, username);
  Rio_writen(fd, slogin.messageString(), slogin.payloadSize());
  
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string x;
  std::string slash;
  std::string command;
  int col;
  while(cin >> x) {
    slash = x.substr(0, 1);
    if (slash == "/") {
      command = (x.substr(1, x.length()));
      handleCommand(fd, command);
    }
    else {
      Message message = Message(TAG_SENDALL, x);
      Rio_writen(fd, message.messageString(), message.payloadSize());
    }



  }


  return 0;
}

// Temp function, we may need to move this into the while loop
void handleCommand(int fd, std::string command) {
  if (command == "join") {
    // You need to get the party's name
    try {
      // Read the message inputted by the user
      std::string party = command.substr(0, 4);

      // Create the message to send to client telling which party is joined
      Message joinParty = Message(TAG_JOIN, party);
      Rio_writen(fd, joinParty.messageString(), joinParty.payloadSize());

      // TODO: somehow get the user into a room
    } catch (const std::out_of_range& orr) {
      // Print need party error and quit the server
    }
    

  }
  else if (command == "leave") {
    try {
      Message leaveParty = Message(TAG_LEAVE, "left room");
      Rio_writen(fd, leaveParty.messageString(), leaveParty.payloadSize());
    } catch (const std::out_of_range& orr) { //TODO: exception is placeholder, need to replace with exception suitable for this error
      // Print whatever error is and exit this function
    }
  }
  else if (command == "quit") {
    try {
      Message quitClient = Message(TAG_QUIT, "quit client");
      Rio_writen(fd, quitClient.messageString(), quitClient.payloadSize());
      // TODO: exit the while loop



    } catch (const std::out_of_range& orr) { //TODO: exception is placeholder, need to replace with exception suitable for this error
      // Print whatever error is and exit this function
    }

    

  }
  else {
    // TODO: handle error with false commands
  }
}

