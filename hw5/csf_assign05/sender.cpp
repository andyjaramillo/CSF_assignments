#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"



// int create_client_socket(const char* hostname, int port) {
//     // create a socket
//     struct sockaddr_in serveraddr = {0};
//     int client_socket = socket(AF_INET, SOCK_STREAM, 0);
//     if (client_socket == -1) {
//         std::cerr << "Failed to create socket\n";
//         return -1;
//     }

//     // get the server address from its hostname
//     serveraddr.sin_family = AF_INET;
//     serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     serveraddr.sin_port = htons((unsigned short)port);

//     if (connect(client_socket, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
//         std::cerr << "Failed to connect to server\n";
//         close(client_socket);
//         return -1;
//     }

//     // return the client socket file descriptor
//     return client_socket;
// }

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

  // std::string output;
  // output = "sender_test-received.out";
  // std::ofstream ofs;
  // ofs.open(output);

   Connection* connection;
  // TODO: connect to server
  // int fd = create_client_socket(server_hostname.c_str(), server_port);
   
  //int fd = open_clientfd(server_hostname.c_str(), argv[2] );
   connection = new Connection();
   connection->connect(server_hostname,server_port);
  // // TODO: send slogin message
 const Message* msg = new Message("slogin", username);
  //   //slogin message
   connection->send(*msg);
  // TODO: loop reading commands from user, sending messages to
    while (connection->is_open()) {  
      Message* sendOut;
      std::string input;
      getline(std::cin, input);
     // 
      size_t space_pos = input.find(" ");
      std::string substring = input.substr(0, space_pos);
      std::string info = input.substr(space_pos + 1);
    
      
      if (input.find("/join") != std::string::npos) {
        sendOut = new Message(TAG_JOIN, info);
     //   sendOut->tag = TAG_JOIN;
    //    sendOut->data = info;
       
        // sendOut = new Message(substring, info);
      } else if (input.find("/leave") != std::string::npos) {
        sendOut = new Message(TAG_LEAVE, "");
   //     sendOut->tag = TAG_LEAVE;
 //       sendOut->data = info;
        // sendOut = new Message(substring, info);
      } else if (input == "/quit") {
          //close(fd); // close server socket
          sendOut = new Message(TAG_QUIT, "bye");
         //  printf("%s", input.c_str());
         // sendOut->tag = TAG_QUIT;
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
        // sendOut->tag = TAG_SENDALL;
       //  sendOut->data = input;
       //   printf("%s",sendOut->data.c_str());
          // sendOut = new Message(substring, input);
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
