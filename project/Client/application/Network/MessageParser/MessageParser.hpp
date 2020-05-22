#pragma once

#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>

struct PlayerInfo;

namespace Network {

class Message;
enum class Status;
enum class RoomStatus;

class MessageParser {
public:
  static std::unique_ptr<Message> ParseToMessage(std::string rawData);
  static std::string ParseToJson(std::unique_ptr<Message> msg);

private:
  // NOTE(vendroid): Request messages
  static std::string InitRequestToJson(std::unique_ptr<Message> msg);
  static std::string ConnectRequestToJson(std::unique_ptr<Message> msg);
  static std::string StartGameSessionRequestToJson(std::unique_ptr<Message> msg);
  static std::string RoomStatusRequestToJson(std::unique_ptr<Message> msg);
  static std::string GetTextRequestToJson(std::unique_ptr<Message> msg);
  static std::string ValidateWrittenTextRequestToJson(std::unique_ptr<Message> msg);
  static std::string ErrorRequestToJson(std::unique_ptr<Message> msg);

  // NOTE(vendroid): Response messages
  static std::unique_ptr<Message> InitResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> ConnectResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> StartGameSessionResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> GetTextResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> RoomStatusResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> ValidateWrittenTextResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> ErrorResponseToMessage(boost::property_tree::ptree& pt);


private:
  static std::pair<Status, std::string> ParseErrorAndStatus(boost::property_tree::ptree& pt);
  static RoomStatus ParsePlayerStatus(const std::string& stringPlayerStatus);
  static std::unordered_map<std::string, PlayerInfo> ParsePlayersInfo(boost::property_tree::ptree& pt);
};

}  // namespace Network