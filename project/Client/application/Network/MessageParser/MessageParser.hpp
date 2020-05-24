#pragma once

#include <string>
#include <tuple>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>

namespace Network {

struct PlayerInfo;
class Message;
enum class Status;
enum class RoomStatus;

class MessageParser {
public:
  std::unique_ptr<Message> ParseToMessage(std::string rawData);
  std::string ParseToJson(std::unique_ptr<Message> msg);

private:
  // NOTE(vendroid): Request messages
  static std::string initRequestToJson(std::unique_ptr<Message> msg);
  static std::string connectRequestToJson(std::unique_ptr<Message> msg);
  static std::string startGameSessionRequestToJson(std::unique_ptr<Message> msg);
  static std::string roomStatusRequestToJson(std::unique_ptr<Message> msg);
  static std::string getTextRequestToJson(std::unique_ptr<Message> msg);
  static std::string validateWrittenTextRequestToJson(std::unique_ptr<Message> msg);
  static std::string leaveRoomRequestToJson(std::unique_ptr<Message> msg);
  static std::string errorRequestToJson(std::unique_ptr<Message> msg);

  // NOTE(vendroid): Response messages
  static std::unique_ptr<Message> initResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> connectResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> startGameSessionResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> getTextResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> roomStatusResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> validateWrittenTextResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> leaveRoomResponseToMessage(boost::property_tree::ptree& pt);
  static std::unique_ptr<Message> errorResponseToMessage(boost::property_tree::ptree& pt);


private:
  static std::pair<Status, std::string> parseErrorAndStatus(boost::property_tree::ptree& pt);
  static RoomStatus parseRoomStatus(const std::string& stringPlayerStatus);
  static std::unordered_map<std::string, PlayerInfo> parsePlayersInfo(boost::property_tree::ptree& pt);
};

}  // namespace Network