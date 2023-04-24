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

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions

////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnInfo *info = (struct ConnInfo*) arg;
  Connection* connection = new Connection(info->clientfd);

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message message;
  while(!connection->receive(message)) {

  }
  std::string room_name;
  

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if (message.tag == TAG_RLOGIN) {
    chat_with_receiver(connection, room_name);
  }

  else if (message.tag == TAG_SLOGIN) {
    chat_with_sender(connection);
  }

  connection->close();
  free(info);


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
  int mcreate = pthread_mutex_init(&m_lock, nullptr);

}

Server::~Server() {
  // TODO: destroy mutex
  int mdestroy = pthread_mutex_destroy(&m_lock);
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
    info->clientfd = clientfd;

    pthread_t thr_id;
    if (pthread_create(&thr_id, NULL, worker, info)) {
      std::cout << "Client won't create" << std::endl;
    }
  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  
}

struct ConnInfo {
  int clientfd;
};

void chat_with_sender(Connection *connection) {
  std::string input;
  std::string party;
  bool exitTrue = false;
  Message message;
  connection->check_end_loop(message, input, party, exitTrue);
}

void chat_with_receiver(Connection *connection, std::string room_name) {
  Message message;
  connection->receiver_loop(message, room_name);
}