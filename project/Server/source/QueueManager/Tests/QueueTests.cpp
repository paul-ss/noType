//
// Created by paul_s on 05.05.2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Queue.hpp"
#include <thread>
#include <set>


class FakeCommand : public Command {
public:
  explicit FakeCommand(const std::string &data) : importantData(data) {}
  std::string importantData;
};


TEST (Queue, push) {
  Queue q;
  q.push(std::make_shared<FakeCommand>("data1"));
  q.push(std::make_shared<FakeCommand>("data2"));
  q.push(std::make_shared<FakeCommand>("data3"));

  auto cmd = std::make_shared<Command>();

  ASSERT_TRUE(q.pop(cmd));
  ASSERT_EQ(std::dynamic_pointer_cast<FakeCommand>(cmd)->importantData, "data1");
  ASSERT_TRUE(q.pop(cmd));
  ASSERT_EQ(std::dynamic_pointer_cast<FakeCommand>(cmd)->importantData, "data2");
  ASSERT_TRUE(q.pop(cmd));
  ASSERT_EQ(std::dynamic_pointer_cast<FakeCommand>(cmd)->importantData, "data3");
}



TEST (Queue, pop) {
  Queue q;
  std::thread t([&]() {
    auto cmd = std::make_shared<Command>();
    ASSERT_TRUE(q.pop(cmd));
    ASSERT_EQ(std::dynamic_pointer_cast<FakeCommand>(cmd)->importantData, "data");
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  q.push(std::make_shared<FakeCommand>("data"));

  t.join();
}



TEST (Queue, notify) {
  Queue q;
  std::thread t([&]() {
    auto cmd = std::make_shared<Command>();
    ASSERT_FALSE(q.pop(cmd));
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  q.notify();

  t.join();
}



TEST (Queue, notify_all) {
  Queue q;
  std::vector<std::thread> thr;
  for (int i = 0; i < 5; i++) {
    thr.emplace_back([&]() {
      auto cmd = std::make_shared<Command>();
      ASSERT_FALSE(q.pop(cmd));
    });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  q.notify();

  for (int i = 0; i < 5; i++) {
    thr.emplace_back([&]() {
      auto cmd = std::make_shared<Command>();
      ASSERT_FALSE(q.pop(cmd));
    });
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  q.notify();


  for (auto &t :thr) {
    t.join();
  }
}



TEST (Queue, multithread) {
  Queue q;
  std::vector<std::thread> thr;
  std::set<std::string> dataSet;
  std::mutex mutex;

  for (int i = 0; i < 10; i++) {
    thr.emplace_back([&, i]() {
      q.push(std::make_shared<FakeCommand>("data" + std::to_string(i)));
    });
  }


  for (int i = 0; i < 10; i++) {
    thr.emplace_back([&]() {
      auto cmd = std::make_shared<Command>();
      ASSERT_TRUE(q.pop(cmd));
      std::string data = std::dynamic_pointer_cast<FakeCommand>(cmd)->importantData;

      std::unique_lock<std::mutex> lock(mutex);
      ASSERT_TRUE(dataSet.emplace(data).second);
    });
  }

  for (auto &t :thr) {
    t.join();
  }

  ASSERT_EQ(dataSet.size(), 10);
  for (int i = 0; i < 10; i++) {
    ASSERT_EQ(dataSet.count("data" + std::to_string(i)), 1);
  }
}
