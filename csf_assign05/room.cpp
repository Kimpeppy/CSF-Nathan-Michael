#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include <pthread.h>
#include <iostream>
#include "client_util.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, nullptr);
}

Room::~Room() {
  // TODO: destroy the mutex
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
  std::string username_copy = sender_username;
  std::string message_text_copy = message_text;

  username_copy = trim(username_copy);
  message_text_copy = trim(message_text_copy);


  std::string payload = room_name + ":" + username_copy + ":" + message_text_copy;
  Guard g(lock);
  for (User *user: members) {
    Message *message = new Message(TAG_DELIVERY, payload);
    user->mqueue.enqueue(message);
  }
}
