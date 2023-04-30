#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"
#include <fstream>


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

  conn = new Connection();
  conn->connect(server_hostname,server_port);
  const Message msg =  Message("rlogin", username);


  conn->send(msg);
  const Message msg2 =  Message("join", room_name);
  conn->send(msg2);



  int keep_going = 1;
  
  while (keep_going) {
      
    Message* incoming = new Message();
    bool client_fd = conn->receive(*incoming);
     
    if (incoming->tag == TAG_DELIVERY) {
      ssize_t first = incoming->data.find(":");
      ssize_t second = incoming->data.find(":", first+1);
      incoming->tag = incoming->data.substr(first+1, second-first-1);
      incoming->data = incoming->data.substr(second+1);
      incoming->data = incoming->data.substr(incoming->data.find(":")+1);
      incoming->data = incoming->data.substr(incoming->data.find(":")+1);
      incoming->tag += incoming->data.substr(0, incoming->data.find(":")+1);
      incoming->data = incoming->data.substr(incoming->data.find(":")+1);
      printf("%s: %s", incoming->tag.c_str(), incoming->data.c_str());
    }
    
    if (client_fd) {
      keep_going = 1; 
            
    } else {
      keep_going = 0;
      close(client_fd);
      }
    }
  return 0;
}
