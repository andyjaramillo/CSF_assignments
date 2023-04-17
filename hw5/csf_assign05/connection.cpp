#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <unistd.h>

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  rio_readinitb(&m_fdbuf, fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  const char* hostname_c_str = hostname.c_str();
  std::string port_string = std::to_string(port);
  const char* port_c_str = port_string.c_str();
  m_fd = open_clientfd(hostname_c_str, port_c_str);
  if (m_fd < 0) {
    m_last_result = Result(EOF_OR_ERROR);
  }
  rio_readinitb(&m_fdbuf, m_fd);
  
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  Close(m_fd);
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  if(m_fd < 0) {
    return false;
  }
  return true;
}

void Connection::close() {
  // TODO: close the connection if it is open
  Close(m_fd);
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  std::string tag = msg.tag;
  std::string data = msg.data;
  std::string total = tag + ":" + data + "\n";
  const char* message_c_string = total.c_str();
  int returnCode = rio_writen(m_fd, message_c_string, total.length());
  if (returnCode < 0) {
    return false;
  } 
  return true;

}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  char buf[MAXLINE];
  ssize_t rc = rio_readlineb(&m_fdbuf, buf, sizeof(buf));

  if (rc <= 0) {
    m_last_result = Result(EOF_OR_ERROR);
  return false;// error reading data from client
  } else {
    //  std::string s(buf);
    // std::stringstream ss(s);
    // std::string tag;
    // std::string data;
    // getline(ss, tag, ':');
    // getline(ss, data);
    // Message *mes = new Message(tag, data);
 //   return mes;
    msg = *(msg.bufferToMessage(buf));

    return true;
  }
}

