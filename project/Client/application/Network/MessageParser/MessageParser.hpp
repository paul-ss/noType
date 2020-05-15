#pragma once

#include <string>
#include <tuple>

#include <boost/property_tree/ptree.hpp>

namespace Network {

class Message;

class MessageParser {
public:
  MessageParser() = default;

  std::unique_ptr<Message> ParseToMessage(std::string rawData);
  std::string ParseToJson(std::unique_ptr<Message> msg);

private:
  struct StringMessageType {
    static const std::string InitRequest                  ("InitRequest");
    static const std::string InitResponse                 ("InitResponse");
    static const std::string ConnectRequest               ("ConnectRequest");
    static const std::string ConnectResponse              ("ConnectResponse");
    static const std::string StartGameSessionRequest      ("StartGameSessionRequest");
    static const std::string StartGameSessionResponse     ("StartGameSessionResponse");
    static const std::string GetTextRequest               ("GetTextRequest");
    static const std::string GetTextResponse              ("GetTextResponse");
    static const std::string RoomStatusRequest            ("RoomStatusRequest");
    static const std::string RoomStatusResponse           ("RoomStatusResponse");
    static const std::string ValidateWrittenTextRequest   ("ValidateWrittenTextRequest");
    static const std::string ValidateWrittenTextResponse  ("ValidateWrittenTextResponse");
    static const std::string Error                        ("Error");
  };

  struct JsonFields {
    // NOTE(vendroid): Общие поля
    static const std::string CommandType       ("commandType");
    static const std::string Status            ("status");
    static const std::string ErrorMessage      ("errorMsg");
    static const std::string Success           ("Success");
    static const std::string Fail              ("Fail");

    // NOTE(vendroid): Основные поля
    static const std::string ClientUuid        ("clientUUID");
    static const std::string Name              ("name");
    static const std::string WaitTime          ("waitTime");
    static const std::string Text              ("text");
    static const std::string TimeFromStart     ("TimeFromStart");
    static const std::string WrittenText       ("writtenText");
    static const std::string RightCount        ("rightCount");

    // NOTE(vendroid): Поля сущности игрока
    struct PlayerInfoFields {
      static const std::string PlayerId        ("playerID");
      static const std::string Name            ("name");
      static const std::string Position        ("position");
      static const std::string Speed           ("speed");
      static const std::string Status          ("status");
      static const std::string Player          ("player");
      static const std::string Winner          ("winner");
      static const std::string Lost            ("lost");
    };
  };

private:
  // NOTE(vendroid): Request messages
  static std::string InitRequestToJson(std::unique_ptr<Message> msg);
  static std::string InitResponseToJson(std::unique_ptr<Message> msg);
  static std::string StartGameSessionRequestToJson(std::unique_ptr<Message> msg);
  static std::string RoomStatusRequestToJson(std::unique_ptr<Message> msg);
  static std::string GetTextRequestToJson(std::unique_ptr<Message> msg);
  static std::string ValidateWrittenTextRequestToJson(std::unique_ptr<Message> msg);

  // NOTE(vendroid): Response messages
  static std::unique_ptr<Message> InitResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> ConnectResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> StartGameSessionResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> GetTextResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> RoomStatusResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> ValidateWrittenTextResponseToMessage(boost::property_tree::ptree& pt);

private:
  static std::pair<Status, std::string> ParseErrorAndStatus(boost::property_tree::ptree& pt);
};

}  // namespace Network