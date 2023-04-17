#include <sstream>
#include <cctype>
#include <cassert>
#include <iostream>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}


Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
    rio_readinitb(&m_fdbuf, m_fd);
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  std::string p = std::to_string(port);
  const char* portString = p.c_str();
  const char* hostNameString = p.c_str();
  int fd = open_clientfd(hostNameString, portString);
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);

  // p = toString(port);
  // port_cstr = c_str(p); //TEST THIS REALLY WORKS
  // int fd = open_clientfd(hostname, port_cstr); //wrong since not using a pointer
  // rio_readinitb(m_fdbuf, fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd > 0);
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
  m_fd = -1;
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string message = msg.tag + ":" + msg.data;
  const char* cMessage = message.c_str();
  
  ssize_t check = rio_writen(m_fd, cMessage, sizeof(cMessage));
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
  if (check <= -1) {
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
  std::string payload = bufString.substr(colonLocation + 1); //colonpos to n
  msg.tag = tag;
  msg.data = payload;


  return syn_ack(tag);
   
  }

bool Connection::syn_ack(std::string tag) {
  if(tag == TAG_OK) {
    m_last_result = SUCCESS; //message format is message_text
    return true; 
  } else if (tag == TAG_ERR) {
    m_last_result = EOF_OR_ERROR; //message format is message_text
    return false;
  }
}

