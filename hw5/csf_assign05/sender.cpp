#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int create_server_socket(int port) {
  struct sockaddr_in serveraddr = {0};
  int ssock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (ssock_fd < 0)
    //fatal("socket failed");
    return -1;
    // m_last_result = Result(EOF_OR_ERROR);

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port);
  
  if (bind(ssock_fd, (struct sockaddr *) &serveraddr,
          sizeof(serveraddr)) < 0)
      return -1;
      //m_last_result = Result(EOF_OR_ERROR);
  if (listen(ssock_fd, 5) < 0) return -1;// m_last_result = Result(EOF_OR_ERROR);;
  return ssock_fd;
}


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
  int fd = create_server_socket(server_port);
  Connection* connection = new Connection(fd);
    connection->connect(server_hostname,server_port);
  // TODO: send slogin message
    const Message* msg = new Message("slogin", username);
    connection->send(*msg);
    std::cout << msg->tag << ":" << msg->data << std::endl;
  
  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate

  return 0;
}
