#include <sstream>
#include <cctype>
#include <cassert>
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
    rio_readinitb(m_fdbuf, m_fd);
  // TODO: call rio_readinitb to initialize the rio_t object
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  p = toString(port);
  port_cstr = c_str(p); //TEST THIS REALLY WORKS
  int fd = open_clientfd(hostname, port_cstr); //wrong since not using a pointer
  rio_readinitb(m_fdbuf, fd)
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return (m_fd > 0)
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
}
