#pragma once

#include <thread>
#include <mutex>
#include <boost/asio.hpp>

namespace Network {

class INetworkManager {
public:
  virtual void Connect() = 0;
  virtual void Disconnect() = 0;
  virtual void Run() = 0;

  virtual ~INetworkManager() = default;

};

class MessageParser;

namespace Connector {

class IQueueManager;

} // namespace Connector

class NetworkManager : public INetworkManager {
public:
  NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager);
  NetworkManager(std::shared_ptr<Connector::IQueueManager> queueManager,
                 std::string serverIp,
                 std::uint32_t port);

  virtual void Connect() override;
  virtual void Disconnect() override;
  virtual void Run() override;

  ~NetworkManager();

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

  bool _isWorking = true;

};

}  // namespace Network
