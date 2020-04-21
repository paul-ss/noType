//
// Created by paul_s on 19.04.2020.
//

#pragma once

#include "Command.hpp"

#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

class Queue {
public:
  Queue() :
      _notified(false) {}
  void push(const std::shared_ptr<Command> &command);
  bool pop(std::shared_ptr<Command> &command);
  void notify();

private:
  std::queue <std::shared_ptr<Command>> _queue;
  std::mutex _queueMutex;
  std::condition_variable _queueCheck;
  bool _notified;
};

