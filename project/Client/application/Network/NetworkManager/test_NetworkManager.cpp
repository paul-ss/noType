#include "Network.hpp"

#include <gtest/gtest.h>

#include <thread>

TEST(NetworkManager, NetworkManagerCheck) {
  auto queueManager
    = std::make_shared<Network::QueueManager>();

  std::shared_ptr<Network::INetworkManager> networkManager = std::make_shared<Network::NetworkManager>(queueManager);
  try {
    networkManager->Connect();
    networkManager->Run();
  } catch (const boost::system::system_error& e) {
    std::cout << e.what() << std::endl;
  }
  Network::InitRequest initRequest;

  {
    auto msg = std::make_unique<Network::Message>(Network::MessageType::InitRequest, initRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto initResponse = std::any_cast<Network::InitResponse>(data);
     std::cout << initResponse.id << std::endl;
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }

  networkManager->Disconnect();
  networkManager->Stop();
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



