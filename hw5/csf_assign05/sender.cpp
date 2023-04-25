#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

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

  Connection* connection; 
  connection = new Connection();
  connection->connect(server_hostname,server_port);
  
  const Message* msg = new Message("slogin", username);
  connection->send(*msg);

  while (connection->is_open()) {  
    Message* sendOut;
    std::string input;
    getline(std::cin, input); 
    size_t space_pos = input.find(" ");
    std::string substring = input.substr(0, space_pos);
    std::string info = input.substr(space_pos + 1);
          
    if (input.find("/join") != std::string::npos) {
      sendOut = new Message(TAG_JOIN, info);
    } else if (input.find("/leave") != std::string::npos) {
      sendOut = new Message(TAG_LEAVE, "");
    } else if (input == "/quit") {
      sendOut = new Message(TAG_QUIT, "bye");
      connection->send(*sendOut);
      Message* error = new Message();
      connection->receive(*error);
      if (error->tag == TAG_ERR) {
        std::cerr << error->data;
        continue;
      }
      break;
    } else {
      sendOut = new Message(TAG_SENDALL, input);
    }
    connection->send(*sendOut);
    Message* response = new Message();
    connection->receive(*response);
    if (response->tag == TAG_ERR) {
      std::cerr << response->data;
      continue;
    }
  }
  connection->close();
  return 0;
}
