#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <ostream>
#include <iostream>
#include <sstream>

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  lock = PTHREAD_MUTEX_INITIALIZER;
  int ret;
  ret = pthread_mutex_init(&lock, nullptr);
  if (ret < 0) {
    std::cerr << "Mutex initialization failed" << std::endl;
    exit(1);
  }
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_lock(&lock);
  for (User *use : members) {
    delete use;
  }
  pthread_mutex_unlock(&lock);

  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  Guard g(lock);
  members.insert(user);
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  Guard g(lock);
  members.erase(user);
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
   
    
    Guard g(lock);
  for (auto it : members) {
    if (it->username != sender_username) {
      std::string room_name = get_room_name();
      size_t pos = room_name.find('\n');
      if (pos != std::string::npos) {
        room_name.erase(pos, 1);
      }
      std::string sender = sender_username;
      size_t pos1 = sender.find('\n');
      if (pos1 != std::string::npos) {
        sender.erase(pos1, 1);
      }

      std::string message = message_text;
      size_t pos2 = message.find('\n');
      if (pos2 != std::string::npos) {
        message.erase(pos2, 1);
      }

      std::string messageString = room_name + ":" + sender + ":" + message;
      Message broadcastMessage = Message(TAG_DELIVERY, messageString);
      (*it).mqueue.enqueue(&broadcastMessage);
    }
  }
  //empty and free memory

}
