#include "Message.hpp"
#include "QueueManager.hpp"

#include <iostream>

namespace Network {

QueueManager::QueueManager()
: _receivedMessagesNotified{false}, _sendingMessagesNotified{false} {}

QueueManager::~QueueManager() { }

void QueueManager::PushToReceivedData(std::unique_ptr<Message> msg) {
  std::unique_lock<std::mutex> lock(_receivedMessagesMutex);
  _receivedMessages.push(std::move(msg));
}

std::unique_ptr<Message> QueueManager::PopReceivedData() {
  std::unique_lock<std::mutex> lock(_receivedMessagesMutex);
  if (_receivedMessages.empty()) {
    return nullptr;
  }

  auto front = std::move(_receivedMessages.front());
  _receivedMessages.pop();

  return front;
}

void QueueManager::PushToSendingData(std::unique_ptr<Message> msg) {
  _sendingMessages.push(std::move(msg));
  _sendingMessagesNotified = true;
  _sendingMessagesCheck.notify_one();
}

std::unique_ptr<Message> QueueManager::PopSendingData() {
  std::unique_lock<std::mutex> lock(_sendingMessagesMutex);
  _sendingMessagesNotified = false;

  if (_sendingMessages.empty()) {
    _sendingMessagesCheck.wait_for(lock, std::chrono::milliseconds(1000), [&](){
      if (_sendingMessagesNotified) {
        return true;
      }
      return !_sendingMessages.empty();
    });
  }

  if (!_sendingMessages.empty()) {
    auto front = std::move(_sendingMessages.front());
    _sendingMessages.pop();

    return front;
  }

  return nullptr;
}

void QueueManager::Notify() {
  _sendingMessagesNotified = true;
  _sendingMessagesCheck.notify_one();
}

}  // namespace Network
