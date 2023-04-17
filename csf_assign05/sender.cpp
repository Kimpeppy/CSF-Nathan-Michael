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
  Message okStatus = Message(TAG_OK, "blah blah blah");
  int fd = open_clientfd(argv[1], argv[2]);
  connection.connect(server_hostname, server_port);
  if(!connection.is_open()) {
    // TODO: Throw error here and exit code
  }
  
  // TODO: send slogin message
  Message slogin = Message(TAG_SLOGIN, username);
  connection.send(slogin);
  if (!connection.receive(okStatus)) {
    // TODO: Handle error for not logging in and exit code
    return 1;
  }
  
  
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string x;
  std::string slash;
  std::string command;
  int col;
  // while(cin >> x) {
  //   slash = x.substr(0, 1);
  //   if (slash == "/") {
  //     command = (x.substr(1, x.length()));
      
      
  //   }
  //   else {
  //     Message message = Message(TAG_SENDALL, x);
  //     Rio_writen(fd, message.messageString(), message.payloadSize());
      
  //   }



  // }

  try {
    while(cin >> x) {
      slash = x.substr(0, 1);
      if (slash == "/") {
        command = x.substr(1, x.length());
      }
      else {
        Message message = Message(TAG_SENDALL, x);
        connection.send(message);
      }
    }
  } catch (const std::out_of_range& orr) {
    
  }


  return 0;
}


// Return false to terminate loop
bool handleCommand(int fd, std::string command, Connection connection) {
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

      return false;
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
      return false;



    } catch (const std::out_of_range& orr) { //TODO: exception is placeholder, need to replace with exception suitable for this error
      // Print whatever error is and exit this function
    }

    

  }
  else {
    // TODO: handle error with false commands
  }
  return true;
}

