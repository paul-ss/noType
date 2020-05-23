#include "Network.hpp"

#include <gtest/gtest.h>

#include <thread>

std::string roomStatusParser(Network::RoomStatus roomStatus) {
  switch (roomStatus) {
    case Network::RoomStatus::Wait:
      return std::string("wait");
    case Network::RoomStatus::End:
      return std::string("end");
    case Network::RoomStatus::Play:
      return std::string("play");
    default:
      return {};
  }
}

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

  // +++++++++++++++++++ Network::InitRequest ++++++++++++++++++++
  {
    Network::InitRequest initRequest;
    auto msg = std::make_unique<Network::Message>(Network::MessageType::InitRequest, initRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  std::string id;
  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto initResponse = std::any_cast<Network::InitResponse>(data);
      id = initResponse.id;
     std::cout << initResponse.id << std::endl;
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }


  // +++++++++++++++++++  Network::ConnectRequest ++++++++++++++++++++
  {
    Network::ConnectRequest connectRequest = {id};
    auto msg = std::make_unique<Network::Message>(Network::MessageType::ConnectRequest, connectRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto connectResponse = std::any_cast<Network::ConnectResponse>(data);
      std::cout << "connectResponse.status: " <<  static_cast<int>(connectResponse.status) << std::endl;
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }

  std::string playerId;
  // +++++++++++++++++++  Network::StartGameSessionRequest ++++++++++++++++++++
  {
    Network::StartGameSessionRequest startGameSessionRequest = {id};
    auto msg
    = std::make_unique<Network::Message>(Network::MessageType::StartGameSessionRequest, startGameSessionRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto startGameSessionResponse = std::any_cast<Network::StartGameSessionResponse>(data);
      playerId = startGameSessionResponse.playerId;
      std::cout << "startGameSessionResponse.waitTime: " << startGameSessionResponse.waitTime << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(startGameSessionResponse.waitTime));
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // +++++++++++++++++++  Network::GetTextRequest ++++++++++++++++++++
  {
    Network::GetTextRequest getTextRequest = {id};
    auto msg
      = std::make_unique<Network::Message>(Network::MessageType::GetTextRequest, getTextRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto getTextResponse = std::any_cast<Network::GetTextResponse>(data);
      std::cout << "getTextResponse.text: " <<  getTextResponse.text << std::endl;
      std::cout << getTextResponse.text << std::endl;
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // +++++++++++++++++++  Network::GetTextRequest ++++++++++++++++++++
  {
    Network::GetTextRequest getTextRequest = {id};
    auto msg
      = std::make_unique<Network::Message>(Network::MessageType::GetTextRequest, getTextRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto getTextResponse = std::any_cast<Network::GetTextResponse>(data);
      std::cout << "getTextResponse.text: " <<  getTextResponse.text << std::endl;
    } catch (const std::bad_any_cast& e) {
      std::cout << e.what() << std::endl;
    }
  }

  // +++++++++++++++++++  Network::RoomStatusRequest ++++++++++++++++++++
  {
    Network::RoomStatusRequest roomStatusRequest = {id};
    auto msg
      = std::make_unique<Network::Message>(Network::MessageType::RoomStatusRequest, roomStatusRequest);
    queueManager->PushToSendingData(std::move(msg));
  }

  {
    std::unique_ptr<Network::Message> msg = nullptr;
    while (!msg) {
      msg = queueManager->PopReceivedData();
    }

    try {
      auto data = msg->ExtractData();
      auto roomStatusResponse = std::any_cast<Network::RoomStatusResponse>(data);
      std::cout << "RoomStatusRequest.roomStatus: " <<  roomStatusParser(roomStatusResponse.roomStatus) << std::endl;
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



