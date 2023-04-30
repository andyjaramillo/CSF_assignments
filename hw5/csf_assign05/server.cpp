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
   // std::map<int, Connection> connections;
    Server *serv;
    
  };

auto Server::return_RooMap() {
  return m_rooms;
}


////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

void chat_with_sender(Message response, std::string username, ClientData *clientdata) {
  User* user = new User(username);
  bool inRoom = false;
  Room* room = nullptr;
 while (clientdata->connection->is_open()) {
  //bool alreadyInRoom = false;
  Message joinMessage;
  bool ret = clientdata->connection->receive(joinMessage);
  if(!ret) {
    Message error = Message("err", "error in receiving message");
    clientdata->connection->send(error);
    return;
  }else {
     if (joinMessage.tag == TAG_ERR) {
      Message error = Message("err", "error in recieve of message");
      clientdata->connection->send(error);
      return;
    } else if(joinMessage.tag == TAG_JOIN) {
      if (room != nullptr) {
        room->remove_member(user);
        room = clientdata->serv->find_or_create_room(joinMessage.data);
        room->add_member(user);
        Message joinedSecondRoom = Message(TAG_OK, "joined room");
        clientdata->connection->send(joinedSecondRoom);
      } else {
        room = clientdata->serv->find_or_create_room(joinMessage.data);
        room->add_member(user);
        Message joinedFirstRoom = Message(TAG_OK, "joined room");
        clientdata->connection->send(joinedFirstRoom);
        inRoom = true;
      }
    } else if(joinMessage.tag == TAG_LEAVE) {
      if (room != nullptr) {
          room->remove_member(user);
          Message leftRoom = Message(TAG_OK, "left room");
          clientdata->connection->send(leftRoom);
          inRoom = false;
        } else {
          Message leftRoom = Message(TAG_ERR, "not in a room");
          clientdata->connection->send(leftRoom);
        }
    } else if(joinMessage.tag == TAG_QUIT) {
      Message quitMessage = Message(TAG_OK, "bye");
      clientdata->connection->send(quitMessage);
      break;
    } else {
      if (room == nullptr) {
         Message errorMessage = Message(TAG_ERR, "not in a room");
        clientdata->connection->send(errorMessage);
      } else {
        room->broadcast_message(user->username, joinMessage.data);
        Message confirmSend = Message(TAG_OK, "message sent");
        clientdata->connection->send(confirmSend);
      }
      
    }
  }

 }
  
}

void chat_with_reciever(Message response, Server *serv, std::string username, ClientData *clientdata) {
  User* user = new User(username);
  user->isReciever = true;
  Message joinMessage;
  Room* room;
  clientdata->connection->receive(joinMessage);
    if (joinMessage.tag == TAG_ERR) {
      Message error = Message("TAG_ERR", "error in recieve of message");
      clientdata->connection->send(error);
      return;
    } else if (joinMessage.tag == TAG_JOIN) { 
      room = serv->find_or_create_room(joinMessage.data);
      room->add_member(user);
      Message joined = Message(TAG_OK, "joined room");
      clientdata->connection->send(joined);
    } else {
      Message error = Message(TAG_ERR, "Needs to join a room");
      clientdata->connection->send(error);
      return;
    }
   

  while(1) {
    Message* sendAllMessages =  user->mqueue.dequeue();
    if (sendAllMessages != nullptr) {
      if (sendAllMessages->tag == TAG_ERR) {
        Message error = Message("err", "error in recieve of sender message");
        clientdata->connection->send(error);
        break;
      } 
      clientdata->connection->send(*sendAllMessages);
    }
 
    
    
    //find a time to break out of the loop
    delete sendAllMessages;
  }
  close(clientdata->sock_fd);

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
  Message response = Message();
    clientdata->connection->receive(response);
  
    if (response.tag == TAG_ERR) {
      std::cerr << response.data;
      return nullptr;
    }
    /*get rid of new line from user*/
  std::string username = response.data;
  Message logged_in = Message();
  logged_in.tag = "ok";
  logged_in.data = "logged in";
  clientdata->connection->send(logged_in);
  if (response.tag == TAG_RLOGIN) {
    chat_with_reciever(response, clientdata->serv, username, clientdata);
  } else if ((response.tag == TAG_SLOGIN)) {
     chat_with_sender(response, username, clientdata);
  } else {
     Message not_logged_in = Message("TAG_ERR", "User needs to login first");
    clientdata->connection->send(not_logged_in);
  }
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  for (auto i : clientdata->serv->return_RooMap()) {
    delete i.second;
  }
  delete clientdata;
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
  if (ret < 0) {
    std::cerr << "Mutex initialization failed" << std::endl;
    exit(1);
  }
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_lock(&m_lock);
  for (auto iter = m_rooms.begin(); iter != m_rooms.end(); ++iter) {
    delete iter->second;
  }
  pthread_mutex_unlock(&m_lock);
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
     m_ssock = m_fd;
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
  //  Guard g(m_lock);
    connfd = accept(m_ssock, nullptr, nullptr);
    if (connfd < 0) {
      std::cerr << "unable to accept client connection" << std::endl;
      return;
    }
    // connfd, connection , {}, this, pthread_t()
    
    ClientData* clientdata = new ClientData;
    clientdata->sock_fd = connfd;
    clientdata->connection = new Connection(connfd);
    clientdata->serv = this;
   

    pthread_t pthread;
  
    int ret = pthread_create(&pthread, nullptr, worker, clientdata);
    if (ret < 0) {
      std::cerr << "Error: could not create client thread" << std::endl;
      delete clientdata->connection;
      delete clientdata;
      continue;
    }
    delete clientdata->connection;
    delete clientdata;

  }
  close(connfd);
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary
  Guard g(m_lock);
  auto m = m_rooms.find(room_name);
  if (m != m_rooms.end()) {
      return m->second;
  } else {
     Room* room = new Room(room_name);
      m_rooms[room_name] = room;
      return room;
    //return m_rooms[room_name];
  }
 // return nullptr;
}
