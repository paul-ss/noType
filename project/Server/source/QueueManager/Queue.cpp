//
// Created by paul_s on 19.04.2020.
//

#include "Queue.hpp"

void Queue::push(const std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueMutex);
  _queue.push(command);
  _queueCheck.notify_one();
}

bool Queue::pop(std::shared_ptr<Command> &command) {
  std::unique_lock<std::mutex> lock(_queueMutex);
  if (_queue.empty()) {
    _queueCheck.wait(lock, [&]() {
        if (_notified) {
          _notified = false;
          return true;
        } else {
          return !_queue.empty();
        }
    });
  }

  if (!_queue.empty()) {
    command = _queue.front();
    _queue.pop();
    return true;
  } else {
    return false;
  }
}

void Queue::notify() {
  std::unique_lock<std::mutex> lock(_queueMutex);
  _notified = true;
  _queueCheck.notify_one();
}