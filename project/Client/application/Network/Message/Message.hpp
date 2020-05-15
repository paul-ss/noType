#pragma once

#include "PlayerInfo.hpp"

#include <any>

namespace Network {

enum class MessageType {
  InitRequest,
  InitResponse,
  ConnectRequest,
  ConnectResponse,
  StartGameSessionRequest,
  StartGameSessionResponse,
  GetTextRequest,
  GetTextResponse,
  RoomStatusRequest,
  RoomStatusResponse,
  ValidateWrittenTextRequest,
  ValidateWrittenTextResponse,
  Error
};

enum class Status {
  Success,
  Fail
};

struct InitRequest {
  std::string name;
};

struct InitResponse {
  std::string id;
  Status status;
  std::string error;
};

struct ConnectRequest {
  std::string id;
};

struct ConnectResponse {
  Status status;
  std::string error;
};

struct StartGameSessionRequest {
  std::string id;
};

struct StartGameSessionResponse {
  std::string id;
  double waitTime;
  Status status;
  std::string error;
};

struct GetTextRequest {
  std::string id;
};

struct GetTextResponse {
  std::string text;
  Status status;
  std::string error;
};

struct RoomStatusRequest {
  std::string id;
};

struct RoomStatusResponse {
  double timeFromStart;
  std::unordered_map<std::string, PlayerInfo> playersInfo;
  Status status;
  std::string error;
};

struct ValidateWrittenTextRequest {
  std::string id;
  std::string writtenText;
};

struct ValidateWrittenTextResponse {
  std::uint16_t rightCount;
  Status status;
  std::string error;
};

class Message {
public:
  Message(MessageType msg_type, std::any&& data);

  const MessageType& GetMessageType();

  std::any&& ExtractData();

private:
  MessageType _msgType;
  std::any _data;
};

}  // namespace Network