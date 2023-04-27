#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}
// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

struct ConnInfo {
  Server *server;
  Connection *conn;
};

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////


//trminate loop and close client thread if anything fails to send
void chat_with_sender(Connection *connection, Server *server, Message message) {
  std::string username = message.data;
  User *user = new User(username);
  Room *roomObj = nullptr;
  while(true) {
    bool received = connection->receive(message);
    if(received) {
      // Make the room object null
      if(message.tag == TAG_LEAVE) {
        if(roomObj == nullptr) { //what if they try to leave without being in a room
          message.tag = TAG_ERR;
          message.data = "Left without being in a room";
          connection->send(message);
        } 
        roomObj->remove_member(user);
        roomObj = nullptr;
        message.tag = TAG_OK;
        message.data = "Left the room!";
        connection->send(message);
      } 
      // Find and join room
      else if (message.tag == TAG_JOIN) {
        std::string room = message.data;
        roomObj->remove_member(user);
        roomObj = server->find_or_create_room(room);
        roomObj->add_member(user);
        message.tag = TAG_OK;
        message.data = "Joined the room!";
        connection->send(message);
      } 
      // Exit the loop
      else if (message.tag == TAG_QUIT) {
        message.tag = TAG_OK;
        message.data = "Quit!";
        connection->send(message);
        return;
      } 
      // Send a message
      else if (message.tag == TAG_SENDALL) {
        if(roomObj == nullptr) {
          message.tag = TAG_ERR;
          message.data = "Tried to sendall while not in a room";
          connection->send(message);
        } else {
          roomObj->broadcast_message(username, message.data);
          message.tag = TAG_OK;
          message.data = "message sent";
          connection->send(message);
        }
      } else {
        message.tag = TAG_ERR;
        message.data = "Something went wrong";
        connection->send(message);
      }
    } else {
      message.tag = TAG_ERR;
      message.data = "Message not received";
      connection->send(message);
    }
  }
}

//terminate loop and close client thread if anything fails, or if quit is received
void chat_with_receiver(Connection *connection, Server *server, Message message) {
  std::string username = message.data;
  User *user = new User(username);
  Room *roomObj = NULL;
  // If message failed to receive, error will occur
  if (!connection->receive(message)) {
    return;
  }
  message.data = trim(message.data);
  
  if (message.tag == TAG_JOIN) {
    roomObj = server->find_or_create_room(message.data);
    roomObj->add_member(user);
    message.tag = TAG_OK;
    message.data = "Joined " + message.data;
    connection->send(message);
  }
  else {
    message.tag = TAG_ERR;
    message.data = "Something went wrong";
    connection->send(message);
    return;
  }
  while(true) {
    Message *output = user->mqueue.dequeue();
    if (output == nullptr) {
      continue;
    }
    if(!connection->send(*output)) {
      roomObj->remove_member(user);
    }

  }
}

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnInfo *info = (struct ConnInfo*) arg;

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message message;
  info->conn->receive(message);
  Message loginInfo;
  //check tags, if it's RLOGIN or SLOGIN, send response
  if(message.tag == TAG_RLOGIN || message.tag == TAG_SLOGIN) {
    loginInfo = Message(message.tag, message.data);
    message.tag = TAG_OK;
    message.data = "Valid login";
    info->conn->send(message);
  }
  else {
    message.tag = TAG_ERR;
    message.data = "Invalid login";
    info->conn->send(message);
  }

  //ELSE SEND ERROR

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if (loginInfo.tag == TAG_RLOGIN) {
    chat_with_receiver(info->conn, info->server, loginInfo);
  }

  else if (loginInfo.tag == TAG_SLOGIN) {
    chat_with_sender(info->conn, info->server, loginInfo);
  }
  
  delete info->conn;
  delete info; //destructor for connection will be called, destroying the connection
  return nullptr;
}
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, nullptr);

}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // // TODO: use open_listenfd to create the server socket, return true
  // //       if successful, false if not
  std::string portString = std::to_string(m_port);
  const char *portCharArr = portString.c_str();
  m_ssock = open_listenfd(portCharArr);
  if (m_ssock < 0) {
    return false;
  }
  return true;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  while(true) {
    int clientfd = Accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      std::cout << "Client won't accept" << std::endl;
    }

    struct ConnInfo *info = new ConnInfo;
    info->conn = new Connection(clientfd);
    info->server = this;
    //info->clientfd = clientfd;

    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info) != 0) {
      std::cout << "Client won't create" << std::endl;
    }
  }

}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard g(m_lock);

  Room *room;
  RoomMap::iterator itr;

  std::string room_name_copy = room_name;
  room_name_copy = trim(room_name_copy);

  itr = m_rooms.find(room_name_copy);

  if (itr == m_rooms.end()) {
    room = new Room(room_name_copy);
    m_rooms.insert({room_name_copy, room});
  }
  else {
    room = itr->second;
  }
  return room;
}



