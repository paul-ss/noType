#pragma once

#include "Message.hpp"
#include "MessageParser.hpp"
#include "QueueManager.hpp"

#include "NetworkManagerConfig.hpp"
#include "NetworkManager.hpp"

#include <boost/system/system_error.hpp>

namespace Network {

NetworkManager::NetworkManager(const std::shared_ptr<Connector::IQueueManager>& queueManager)
                              NetworkManager(queueManager, kServerIp, kServerPort) : {}

NetworkManager::NetworkManager(const std::shared_ptr<Connector::IQueueManager>& queueManager,
                               std::string serverIp,
                               std::uint32_t port) :
                                 _queueManager{queueManager}
                                 _ioService{};
                                 _endPoint(boost::asio::ip::address::from_string(serverIp), port),
                                 _socket(_ioService) {};

NetworkManager::~NetworkManager() {
  _thread->join();
}

void NetworkManager::Connect() {
  try {
    _socket.connect(_endPoint);
  } catch (const boost::system::system_error& error) {
    // TODO(vendroid): Логирование!
    std::cout << error.code() << std::endl;
  }
}


void NetworkManager::Disconnect() {
  std::unique_lock<std::mutex> lock(_networkManagerMutex);
  _ioService.stop();
  _isWorking = false;
}

void NetworkManager::Run() {
  auto work = [this](){ this->loop() };
  _isWorking = true;
  _thread = std::make_unique<std::thread>(work);
}

void NetworkManager::loop() {
  while (true) {
    {
      std::unique_lock<std::mutex> lock(_networkManagerMutex);
      if (!_isWorking) {
        break;
      }
    }

    if (auto msg = _queueManager->PopSendingData(); msg != nullptr ) {
      std::string jsonData = _messageParser->ParseToJson(std::move(msg));

      boost::asio::write(_socket, boost::asio::buffer(jsonData + kDelimiter));
    }

    boost::asio::streambuf buf;
    boost::asio::read_until(_socket, buf, kDelimiter);
    std::string jsonData = boost::asio::buffer_cast<const char*>(buf);

    auto msg = _messageParser->ParseToMessage(jsonData);
    _queueManager->PushToReceivedData(std::move(jsonData));
  }
}

}  // namespace Network