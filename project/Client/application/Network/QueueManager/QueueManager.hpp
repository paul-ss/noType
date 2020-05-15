#pragma once

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

class Message;

namespace Network {

namespace Client {

class IQueueManager {
public:
  virtual std::unique_ptr<Message> void PopReceivedData() = 0;
  virtual void PushToSendingData(std::unique_ptr<Message> msg) = 0;
  virtual ~IQueueManager() = default;
};

}  // namespace Client

namespace Connector {

class IQueueManager {
public:
  virtual std::unique_ptr<Message> void PopSendingData() = 0;
  virtual void PushToReceivedData(std::unique_ptr<Message> msg) = 0;
  virtual ~IQueueManager() = default;
};

}  // namespace Connector

class QueueManager : public Client::IQueueManager, public Connector::IQueueManager {
public:
  QueueManager() = default;

  std::unique_ptr<Message> void PopReceivedData() override;
  void PushToSendingData(std::unique_ptr<Message> msg) override;

  std::unique_ptr<Message> void PopSendingData() override;
  void PushToReceivedData(std::unique_ptr<Message> msg) override;

private:
  std::queue<std::unique_ptr<Message>> _receivedMessages;
  std::queue<std::unique_ptr<Message>> _sendingMessages;

private:
  std::mutex _receivedMessagesMutex;
  std::mutex _sendingMessagesMutex;

  std::condition_variable _receivedMessagesCheck;
  std::condition_variable _sendingMessagesCheck;

  bool _receivedMessagesNotified;
  bool _sendingMessagesNotified;
};

}  // namespace Network