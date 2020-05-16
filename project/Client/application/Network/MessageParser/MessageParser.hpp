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
  // NOTE(vendroid): Request messages
  std::string InitRequestToJson(std::unique_ptr<Message> msg);
  std::string InitResponseToJson(std::unique_ptr<Message> msg);
  std::string StartGameSessionRequestToJson(std::unique_ptr<Message> msg);
  std::string RoomStatusRequestToJson(std::unique_ptr<Message> msg);
  std::string GetTextRequestToJson(std::unique_ptr<Message> msg);
  std::string ValidateWrittenTextRequestToJson(std::unique_ptr<Message> msg);

  // NOTE(vendroid): Response messages
  std::unique_ptr<Message> InitResponseToMessage(boost::property_tree::ptree& pt);
  std::unique_ptr<Message> ConnectResponseToMessage(boost::property_tree::ptree& pt);
  std::unique_ptr<Message> StartGameSessionResponseToMessage(boost::property_tree::ptree& pt);
  std::unique_ptr<Message> GetTextResponseToMessage(boost::property_tree::ptree& pt);
  std::unique_ptr<Message> RoomStatusResponseToMessage(boost::property_tree::ptree& pt);
  std::unique_ptr<Message> ValidateWrittenTextResponseToMessage(boost::property_tree::ptree& pt);

private:
  std::pair<Status, std::string> ParseErrorAndStatus(boost::property_tree::ptree& pt);
};

}  // namespace Network