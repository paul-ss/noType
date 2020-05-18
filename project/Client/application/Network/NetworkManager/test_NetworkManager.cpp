#include "Network.hpp"

#include <gtest/gtest.h>

#include <thread>

TEST(NetworkManager, NetworkManagerCheck) {
  auto queueManager
    = std::make_shared<Network::QueueManager>();

  std::shared_ptr<Network::INetworkManager> _networkManager = std::make_shared<Network::NetworkManager>(queueManager);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



