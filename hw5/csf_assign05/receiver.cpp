#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
#include <fstream>
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
  

   std::string output;
    output = "receiver_test-received.out";
    std::ofstream ofs;
    ofs.open(output);
   
  

  Connection* conn;

  // TODO: connect to server
   //  int fd = create_server_socket(server_port);
    conn = new Connection();
     conn->connect(server_hostname,server_port);
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
    const Message* msg = new Message("rlogin", username);
    ofs << msg->tag << ":" << msg->data << std::endl;

    conn->send(*msg);
    const Message* msg2 = new Message("join", room_name);
    conn->send(*msg2);
    ofs << msg2->tag << ":" << msg2->data;
     ofs.close();
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
    int keep_going = 1;
    // std::cout << msg->tag << ":" << msg->data << std::endl;
  
    while (keep_going) {
      
      Message* incoming = new Message();
      // std::cout << msg->tag << ":" << msg->data << std::endl;
      bool client_fd = conn->receive(*incoming);
     
      if (incoming->tag != TAG_OK) {
       
         // printf("hello\n");
          //not the verification lines, now we have to filter for delivery/payload
         // printf("%s", incoming->tag.c_str()); 
          // ssize_t first = incoming->data.find(":");
          // ssize_t second = incoming->data.find(":", first+1);
          // incoming->tag =  incoming->data.substr(first+1, second);
          // incoming->data = incoming->data.substr(second+1);
          // printf("%s %s", incoming->tag.c_str(), incoming->data.c_str()); 
       //   incoming->data = incoming->data.substr(second+1);
      ssize_t first = incoming->data.find(":");
    ssize_t second = incoming->data.find(":", first+1);
    incoming->tag = incoming->data.substr(first+1, second-first-1);
    incoming->data = incoming->data.substr(second+1);
    incoming->data = incoming->data.substr(incoming->data.find(":")+1);
    incoming->data = incoming->data.substr(incoming->data.find(":")+1);
    incoming->tag += incoming->data.substr(0, incoming->data.find(":")+1);
    incoming->data = incoming->data.substr(incoming->data.find(":")+1);
    printf("%s: %s", incoming->tag.c_str(), incoming->data.c_str());
       //   printf("%s", incoming->tag.c_str()); 
      }
     
      // std::cout << msg->tag << ":" << msg->data << std::endl;
      if (client_fd) {
        keep_going = 1; 
            
        } else {
          keep_going = 0;
          close(client_fd);
        }
        }
   
  //  close(); // close server socket

  return 0;
}
