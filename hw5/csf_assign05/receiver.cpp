#include <iostream>
#include <string>
#include <vector>
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
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection* conn;

  // TODO: connect to server
     int fd = create_server_socket(server_port);
     conn = new Connection(fd);
    conn->connect(server_hostname,server_port);
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
    const Message* msg = new Message("rlogin", username);
    
    conn->send(*msg);
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
    int keep_going = 1;
    // std::cout << msg->tag << ":" << msg->data << std::endl;
    while (keep_going) {
      Message* msg = new Message();
      // std::cout << msg->tag << ":" << msg->data << std::endl;
      bool client_fd = conn->receive(*msg);
      // std::cout << msg->tag << ":" << msg->data << std::endl;
      if (client_fd) {
        keep_going = 1;
        //  std::cout << msg->tag << ":" << msg->data << std::endl;
      //  printf("%s  %s",msg->tag, msg->data);
       // fprintf(stdout, "msg is %s : %s", msg->tag, msg->data);
  //      close(client_fd); // close the connection
        } else {
          keep_going = 0;
          close(client_fd);
        }
        }
    close(fd); // close server socket

  return 0;
}
