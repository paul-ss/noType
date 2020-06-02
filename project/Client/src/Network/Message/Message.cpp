#include "Message.hpp"

namespace Network {
  Message::Message(MessageType msgType, std::any data) : _msgType(msgType), _data(std::move(data)) {}

  const MessageType& Message::GetMessageType() {
    return _msgType;
  }

  std::any&& Message::ExtractData() {
    return std::move(_data);
  }
}  // namespace Network
