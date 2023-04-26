#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
  pthread_mutexattr_t mattr;
  

  struct ClientData {
    int sock_fd;
    Connection* connection;
    std::vector<Connection> connections;
    Server *serv;
    pthread_t thread;
  };

  struct UserDescrition {
    User user;
    std::string username;
  }
////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

void chat_with_sender(Message* response, Connection* connect) {
  User user = new User;
  
}

void chat_with_reciever(Message* response, Connection* connect) {
  User* user = new User;
  Message* joinMessage = new Message();
  connect->receive(*joinMessage);
    if (joinMessage->tag == TAG_ERR) {
      connect->send()
      return;
    }
    //join a room
    Room room = new Room();
    room.add_member(&user);
    Message* joined = new Message();
    joined->tag = "ok";
    joined->data = "join room";
    connect.send(logged_in);

  while(1) {

    

  }

}


namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
   ClientData *clientdata = static_cast<ClientData*>(arg);

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message* response = new Message();
    connect->receive(*response);
    if (response->tag == TAG_ERR) {
      std::cerr << response->data;
      return;
    }
  Message* logged_in = new Message();
  logged_in->tag = "ok";
  logged_in->data = "logged in";
  connect.send(logged_in);
  if (response->tag == TAG_RLOGIN) {
    chat_with_receiver(response, connect);
  } else if ((response->tag == TAG_SLOGIN)) {
     chat_with_sender(response, connect);
  }
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)

  return nullptr;
}

}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  m_lock = PTHREAD_MUTEX_INITIALIZER;

  int ret;
  ret = pthread_mutex_init(&m_lock, &mattr); 
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string port_string;
  std::stringstream ss;
  ss << m_port;
  ss >> port_string;
     int m_fd = open_listenfd(port_string.c_str());
  if (m_fd < 0) {
     return false;
  }else {
     return true;
  }
 
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  int connfd;

  while(1) {
    Guard g(m_lock);
    connfd = accept(m_ssock, nullptr, nullptr);
    if (connfd < 0) {
      std::cerr << "unable to accept client connection" << std::endl;
      return;
    }

    Connection* connection  = new Connection(connfd);
    // connfd, connection , {}, this, pthread_t()
    ClientData* clientdata = new ClientData;
    clientdata->sock_fd = connfd;
    clientdata->connection = connection;
    clientdata->serv = this;
    clientdata->thread = pthread_t();
    // {connfd, &connection, {}, this, pthread_t()};
    // int *connfd_ptr = malloc(sizeof(int));
    // *connfd_ptr = connfd;
   
    Message* response = new Message();
    connection->receive(*response);
    if (response->tag == TAG_ERR) {
      std::cerr << response->data;
      continue;
    }


    int ret = pthread_create(&clientdata->thread, nullptr, &worker, &clientdata);
    if (ret < 0) {
      std::cerr << "Error: could not create client thread" << std::endl;
      delete clientdata;
      continue;
    } 
    // else {
    //    server_chat_with_client(connfd, &connection);
    //   close(connfd);
    //   exit(0);
    // }
    close(connfd);

  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  return nullptr;
}
