#pragma once

#include <string>

namespace Network {

class Message;

class MessageParser {
  MessageParser() = default;

  std::unique_ptr<Message> ParseToMessage(std::string rawData);
  std::string ParseToJson(std::unique_ptr<Message> msg);

private:
  struct StringMessageType {
    static const std::string InitRequest("InitRequest");
    static const std::string InitResponse("InitResponse");
    static const std::string ConnectRequest("ConnectRequest");
    static const std::string ConnectResponse("ConnectResponse");
    static const std::string StartGameSessionRequest("StartGameSessionRequest");
    static const std::string StartGameSessionResponse("StartGameSessionResponse");
    static const std::string GetTextRequest("GetTextRequest");
    static const std::string GetTextResponse("GetTextResponse");
    static const std::string RoomStatusRequest("RoomStatusRequest");
    static const std::string RoomStatusResponse("RoomStatusResponse");
    static const std::string ValidateWrittenTextRequest("ValidateWrittenTextRequest");
    static const std::string ValidateWrittenTextResponse("ValidateWrittenTextResponse");
    static const std::string Error("Error");
  };

  struct JsonFields {
    // NOTE(vendroid): Общие поля
    static const std::string Status("Status");
    static const std::string ErrorMessage("ErrorMessage");
    static const std::string Success("Success");
    static const std::string Fail("Fail");

    // NOTE(vendroid): Основные поля
    static const std::string Id("Id");
    static const std::string Name("Name");
    static const std::string WaitTime("WaitTime");
    static const std::string Text("Text");
    static const std::string TimeFromStart("TimeFromStart");
    static const std::string WrittenText("WrittenText");
    static const std::string RightCount("RightCount");

    // NOTE(vendroid): Поля сущности игрока
    struct PlayerInfoFields {
      static const std::string Id("Id");
      static const std::string Name("Name");
      static const std::string Position("Position");
      static const std::string Speed("Speed");
      static const std::string Status("Status");
      static const std::string Player("Player");
      static const std::string Winner("Winner");
      static const std::string Lost("Lost");
    };
  };
};

}  // namespace Network