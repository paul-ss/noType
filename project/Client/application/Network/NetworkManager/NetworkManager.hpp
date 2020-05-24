#pragma once

#include <thread>
#include <mutex>
#include <boost/asio.hpp>

#include <future>

namespace Network {

class INetworkManager {
public:
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;
  virtual void Run() = 0;
  virtual void Stop() = 0;

  virtual ~INetworkManager() = default;
};

class MessageParser;

namespace Connector {

class IQueueManager;

} // namespace Connector

class NetworkManager : public INetworkManager {
public:
  explicit NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager);
  NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager,
                 const std::string& serverIp,
                 std::uint32_t port);

  void Connect() override;
  void Disconnect() override;
  void Run() override;
  void Stop() override;

private:
  void loop();

private:
  std::shared_ptr<Connector::IQueueManager> _queueManager;

  boost::asio::io_service _ioService;
  boost::asio::ip::tcp::endpoint _endPoint;
  boost::asio::ip::tcp::socket _socket;

  std::unique_ptr<MessageParser> _messageParser;

  std::unique_ptr<std::thread> _thread;
  std::mutex _networkManagerMutex;

  bool _isWorking;
  bool _isStoped = false;

};

}  // namespace Network
