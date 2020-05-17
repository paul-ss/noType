#include <gtest/gtest.h>

#include "Message.hpp"
#include "QueueManager.hpp"


TEST(QueueManager, pushToSendingData) {
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

TEST(QueueManager, pushToSendingData) {
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

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


