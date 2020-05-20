#include "Message.hpp"
#include "MessageParser.hpp"
#include "QueueManager.hpp"

#include "NetworkManagerConfig.hpp"
#include "NetworkManager.hpp"

#include <iostream>

#include <boost/system/system_error.hpp>

namespace Network {

static void eraseDelimiter(std::string& jsonData, const std::string_view& delimiter);


NetworkManager::NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager) :
                                            NetworkManager(queueManager, std::string(kServerIp), kServerPort)  {}

NetworkManager::NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager,
                               std::string serverIp,
                               std::uint32_t port) :
                                 _queueManager{queueManager},
                                 _ioService{},
                                 _endPoint(boost::asio::ip::address::from_string(serverIp), port),
                                 _socket{_ioService},
                                 _isWorking{false} {}

NetworkManager::~NetworkManager() {
  //if (!_isWorking) {
    _thread->join();
  //}
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
  _queueManager->Notify();
  _isWorking = false;
}

void NetworkManager::Run() {
  auto work = [this](){ this->loop(); };
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

    try {
      if (auto msg = _queueManager->PopSendingData(); msg != nullptr) {
        std::string jsonData = _messageParser->ParseToJson(std::move(msg));

        boost::asio::write(_socket, boost::asio::buffer(jsonData + std::string(kDelimiter)));
      }

      boost::asio::streambuf buf;
      boost::asio::read_until(_socket, buf, kDelimiter);
      std::string jsonData((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());
      eraseDelimiter(jsonData, kDelimiter);

      auto msg = _messageParser->ParseToMessage(jsonData);
      _queueManager->PushToReceivedData(std::move(msg));
    } catch (...) {
      std::cout << "something gone wrong" << std::endl;
      break;
    }
  }
}

static void eraseDelimiter(std::string& jsonData, const std::string_view& delimiter) {
  if (jsonData.size() > delimiter.size()) {
    jsonData = jsonData.substr(0,jsonData.size() - delimiter.size());

    jsonData.shrink_to_fit();
  }
}

}  // namespace Network