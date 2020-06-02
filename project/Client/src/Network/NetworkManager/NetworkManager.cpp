#include "Message.hpp"
#include "MessageParser.hpp"
#include "QueueManager.hpp"

#include "NetworkManagerConfig.hpp"
#include "NetworkManager.hpp"

#include "exceptions.hpp"
#include "logger.hpp"

#include <boost/exception/exception.hpp>
#include <boost/system/system_error.hpp>

namespace Network {

static void eraseDelimiter(std::string& jsonData, const std::string_view& delimiter);


NetworkManager::NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager) :
                                            NetworkManager(std::move(queueManager), std::string(kServerIp), kServerPort)
                                            {}

NetworkManager::NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager,
                               const std::string& serverIp,
                               std::uint32_t port) :
                                 _queueManager{std::move(queueManager)},
                                 _ioService{},
                                 _endPoint(boost::asio::ip::address::from_string(serverIp), port),
                                 _socket{_ioService},
                                 _isWorking{false} {}

void NetworkManager::Connect() {
  try {
    _socket.connect(_endPoint);
  } catch (const boost::system::system_error& error) {
    throw ConnectionFailure(error.what());
  }
}

void NetworkManager::Disconnect() {
 std::unique_lock<std::mutex> lock(_networkManagerMutex);
  _queueManager->Notify();
  _isWorking = false;
}

void NetworkManager::Run() {
  std::unique_lock<std::mutex> lock(_networkManagerMutex);
  auto work = [this](){ this->loop(); };
  _isWorking = true;
  _thread = std::make_unique<std::thread>(work);
}

void NetworkManager::Stop() {
  std::unique_lock<std::mutex> lock(_networkManagerMutex);
  _queueManager->Notify();
  _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
  _socket.close();
  while(!_isStoped) {}
  _thread->join();
}

void NetworkManager::loop() {
  while (true) {
    {
      if (!_isWorking) {
        _isStoped = true;
        break;
      }
    }
    try {
      if (auto sendMsg = _queueManager->PopSendingData(); sendMsg != nullptr) {
        std::string sendJsonData = _messageParser->ParseToJson(std::move(sendMsg));

        boost::asio::write(_socket, boost::asio::buffer(sendJsonData + std::string(kDelimiter)));

        boost::asio::streambuf buf;
        {
          if (!_isWorking) {
            _isStoped = true;
            break;
          }
        }
        boost::asio::read_until(_socket, buf, kDelimiter);
        std::string recvJsonData((std::istreambuf_iterator<char>(&buf)), std::istreambuf_iterator<char>());

        eraseDelimiter(recvJsonData, kDelimiter);
        auto recvMsg = _messageParser->ParseToMessage(recvJsonData);

        _queueManager->PushToReceivedData(std::move(recvMsg));
      }
    } catch (const boost::system::system_error& e) {
      BOOST_LOG_TRIVIAL(error) << "NetworkManager::loop: " << e.what();
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