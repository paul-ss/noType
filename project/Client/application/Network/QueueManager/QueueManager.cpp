#pragma once

#include "QueueManager.hpp"

namespace Network {

QueueManager::QueueManager()
: _receivedMessagesNotified{false}, _sendingMessagesNotified{false} {}

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
  std::unique_lock<std::mutex> lock(_sendingMessagesMutex);
  _sendingMessages.push(std::move(msg));
  _sendingMessagesCheck.notify_one();
}

std::unique_ptr<Message> QueueManager::PopSendingData() {
  std::unique_lock<std::mutex> lock(_sendingMessagesMutex);

  if (_sendingMessagesNotified.empty()) {
    _sendingMessagesCheck.wait(lock, [&](){
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

}  // namespace Network
