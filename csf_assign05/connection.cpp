#include <sstream>
#include <cctype>
#include <cassert>
#include <iostream>
#include "csapp.h"
#include "message.h"
#include "connection.h"

using namespace std;
Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}


Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  
  std::string p = std::to_string(port);
  const char* portString = p.c_str();
  const char* hostNameString = hostname.c_str();
  
  int fd = open_clientfd(hostNameString, portString);
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
  m_fd = fd;
  
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd >= 0);
}

void Connection::close() {
  // TODO: close the connection if it is open
  ::close(m_fd); 
  m_fd = -1;
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string message = msg.tag + ":" + msg.data + "\n";
  const char* cMessage = message.c_str();
  ssize_t check = rio_writen(m_fd, cMessage, message.length());
  if (check <= -1) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  else {
    m_last_result = SUCCESS;
    return true;
  }
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buf[msg.MAX_LEN + 1];
  size_t check = rio_readlineb(&m_fdbuf, buf, sizeof(buf));
  if (check < 0) {
    m_last_result = EOF_OR_ERROR;
    return false;
  }

  buf[msg.MAX_LEN + 1] = '\0';
  std::string bufString(buf);

  // If there is no colon, this message is invalid
  size_t colonLocation = bufString.find(':');
  if (colonLocation == std::string::npos) {
    m_last_result = INVALID_MSG;
    return false;
  }

  std::string tag = bufString.substr(0, colonLocation);
  std::string payload = bufString.substr(colonLocation + 1); //automatically goes to the end

  msg.tag = tag;
  msg.data = payload;

  return syn_ack(msg);
   
  }

bool Connection::syn_ack(Message &msg) {
  if(msg.tag == TAG_OK || msg.tag == TAG_DELIVERY) {
    m_last_result = SUCCESS; //message format is message_text
    return true; 
  } else if (msg.tag == TAG_ERR) {
    m_last_result = EOF_OR_ERROR; //message format is message_text
    std::cerr << msg.data;
    return false;
  }
}

bool Connection::communicate(Message &msg) {
  if (!send(msg)) {
    return false;
  }
  if (!receive(msg)) {
    return false;
  }
  return true;
}

void Connection::check_end_loop(Message &message, std::string input, std::string party, bool exitTrue) {
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
    if (!communicate(message)) {
      exitTrue = false;
    }
    if (exitTrue) {
      break;
    }
  }
}

void Connection::receiver_loop(Message &message, std::string room_name) {
  bool continueLoop = true;
  std::string roomReceiver;
  std::string sender;
  std::string userMessage;
  size_t colon1;
  size_t colon2;
  while (continueLoop) {
    continueLoop = receive(message);
    if (!continueLoop) {
      continue;
    }
    colon1 = message.data.find(':'); 
    colon2 = message.data.find(':', colon1 + 1);
    roomReceiver = message.data.substr(0, colon1); 
    sender = message.data.substr(colon1+1, colon2-colon1 - 1);
    userMessage = message.data.substr(colon2 + 1);
    if (roomReceiver != room_name) {
      continue;
    }
    std::cout << sender << ": " << userMessage;
  }
}