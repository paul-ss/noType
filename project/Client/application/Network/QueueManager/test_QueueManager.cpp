#include "Message.hpp"
#include "QueueManager.hpp"

#include <gtest/gtest.h>

#include <thread>

TEST(QueueManager, PushToSendingData) {
  auto queueManager
    = std::make_unique<Network::QueueManager>();
  {
    Network::InitResponse initResponse = {};
    auto msg = std::make_unique<Network::Message>(Network::MessageType::InitResponse, initResponse);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    auto msg = queueManager->PopSendingData();
    EXPECT_EQ(msg->GetMessageType(), Network::MessageType::InitResponse);
  }
}

TEST(QueueManager, PushToReceivedData) {
  auto queueManager
    = std::make_unique<Network::QueueManager>();

  {
    Network::InitResponse initRequest = {};
    auto msg = std::make_unique<Network::Message>(Network::MessageType::InitRequest, initRequest);
    queueManager->PushToReceivedData(std::move(msg));
  }

  {
    auto msg = queueManager->PopReceivedData();
    EXPECT_EQ(msg->GetMessageType(), Network::MessageType::InitRequest);
  }
}


TEST(QueueManager, PushToSendingDataMultiThreads) {
  auto queueManager
    = std::make_unique<Network::QueueManager>();

  std::thread thread([&]() {
    while (true) {
      if (auto msg = queueManager->PopSendingData()) {
        EXPECT_EQ(msg->GetMessageType(), Network::MessageType::InitResponse);
        break;
      }
      // NOTE(vendroid): Делаем какую-то несложную логику
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  });

  {
    Network::InitResponse initResponse = {};
    auto msg = std::make_unique<Network::Message>(Network::MessageType::InitResponse, initResponse);
    queueManager->PushToSendingData(std::move(msg));
  }

  thread.join();
}


TEST(QueueManager, TestForArtem) {

}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


