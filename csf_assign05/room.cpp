#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <pthread.h>
#include <iostream>

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  int mcreate = pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // TODO: destroy the mutex
  int mdestroy = pthread_mutex_destroy(&lock);
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
  std::cout << members.size() << std::endl;
  for (User *user: members) {
    std::cout << "here is line 36" << std::endl;
    Message *message = new Message(sender_username, message_text);
    user->mqueue.enqueue(message);
  }
}
