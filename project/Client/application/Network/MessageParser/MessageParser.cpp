#include "MessageParser.hpp"
#include "Message.hpp"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

namespace Network {

std::string MessageParser::ParseToJson(std::unique_ptr<Message> msg) {
  if (!msg) {
    // TODO(vendroid): кинуть исключение
    std::cout << "Invalid pointer [MessageParser::ParseToJson]" << std::endl;
    return {};
  }

  std::string res;
  switch (msg->GetMessageType()) {
    case MessageType::InitRequest: {
      res = InitRequestToJson(std::move(msg));
    }
    case MessageType::ConnectRequest: {
      res = ConnectRequestToJson(std::move(msg));
    }
    case MessageType::StartGameSessionRequest:{
      res = StartGameSessionRequestToJson(std::move(msg));
    }
    case MessageType::GetTextRequest: {
      res = GetTextRequestToJson(std::move(msg));
    }
    case MessageType::RoomStatusRequest: {
      res = RoomStatusRequestToJson(std::move(msg));
    }
    case MessageType::ValidateWrittenTextRequest: {
      res = ValidateWrittenTextRequestToJson(std::move(msg));
    }
  }

  return res;
}

std::unique_ptr<Message> MessageParser::ParseToMessage(std::string rawData) {
  boost::property_tree::ptree pt;

  std::stringstream data(rawData);
  boost::property_tree::read_json(data, pt);

  try {
    auto stringMessageType = pt.get<std::string>(JsonFields::CommandType);
    if (stringMessageType == StringMessageType::InitResponse) {
      return InitResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ConnectResponse) {

    } else if (stringMessageType == StringMessageType::ConnectResponse) {

    }
  } catch (const boost::property_tree::ptree_error& error) {
    std::cout << "[MessageParser::ParseToMessage]: " << error.what() << std::endl;
    return nullptr;
  }

}

static std::string MessageParser::InitRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto initRequestData = std::any_cast<InitRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::InitRequest);
  pt.put(JsonFields::Name, initRequestData.name);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

static std::string MessageParser::ConnectRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto сonnectRequestData = std::any_cast<ConnectRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::ConnectRequest);
  pt.put(JsonFields::ClientUuid, сonnectRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

static std::string MessageParser::StartGameSessionRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto startGameSessionRequestData = std::any_cast<StartGameSessionRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::StartGameSessionRequest);
  pt.put(JsonFields::ClientUuid, startGameSessionRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

static std::string MessageParser::RoomStatusRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto roomStatusRequestData = std::any_cast<RoomStatusRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::RoomStatusRequest);
  pt.put(JsonFields::ClientUuid, roomStatusRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

static std::string MessageParser::GetTextRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto getTextRequestData = std::any_cast<GetTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::GetTextRequest);
  pt.put(JsonFields::ClientUuid, getTextRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

static std::string MessageParser::ValidateWrittenTextRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto validateWrittenTextRequestData = std::any_cast<ValidateWrittenTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::ValidateWrittenTextRequest);
  pt.put(JsonFields::ClientUuid, validateWrittenTextRequestData.id);
  pt.put(JsonFields::WrittenText, validateWrittenTextRequestData.writtenText);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

// =====================================================================================================================

std::unique_ptr<Message> MessageParser::InitResponseToMessage(boost::property_tree::ptree &pt) {
  auto id = pt.get<std::string>(JsonFields::ClientUuid);
  auto errorData = ParseErrorAndStatus(pt);

  InitResponse initResponse = {id, errorData.first, errorData.second};
  std::any data = initResponse;
  return std::make_unique<Message>(MessageType::InitResponse, std::make_any(initResponse));
}

std::unique_ptr<Message> MessageParser::ConnectResponseToMessage(boost::property_tree::ptree &pt) {
  auto errorData = ParseErrorAndStatus(pt);

  ConnectResponse connectResponse = {errorData.first, errorData.second};
  std::any data = connectResponse;
  return std::make_unique<Message>(MessageType::ConnectResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::StartGameSessionResponseToMessage(boost::property_tree::ptree &pt) {
  auto id = pt.get<std::string>(JsonFields::ClientUuid);
  auto waitTime = pt.get<double>(JsonFields::WaitTime);
  auto errorData = ParseErrorAndStatus(pt);

  StartGameSessionResponse startGameSessionResponse = {id, waitTime, errorData.first, errorData.second};
  std::any data = startGameSessionResponse;
  return std::make_unique<Message>(MessageType::StartGameSessionResponse, std::move(data));
}

static std::unique_ptr<Message> MessageParser::GetTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto text = pt.get<std::string>(JsonFields::Text);
  auto errorData = ParseErrorAndStatus(pt);

  GetTextResponse getTextResponse = {text, errorData.first, errorData.second};
  std::any data = getTextResponse;
  return std::make_unique<Message>(MessageType::GetTextResponse, std::move(data));
}

static std::unique_ptr<Message> MessageParser::RoomStatusResponseToMessage(boost::property_tree::ptree& pt) {
  auto timeFromStart = pt.get<double>(JsonFields::Text);
  auto errorData = ParseErrorAndStatus(pt);

  GetTextResponse getTextResponse = {text, errorData.first, errorData.second};
  std::any data = getTextResponse;
  return std::make_unique<Message>(MessageType::RoomStatusResponse, std::move(data));
}

static std::unique_ptr<Message> MessageParser::ValidateWrittenTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto rightCount = pt.get<std::uint16_t>(JsonFields::RightCount);
  auto errorData = ParseErrorAndStatus(pt);

  ValidateWrittenTextResponse validateWrittenTextResponse = {rightCount, errorData.first, errorData.second};
  std::any data = validateWrittenTextResponse;
  return std::make_unique<Message>(MessageType::ValidateWrittenTextResponse, std::move(data));
}

std::pair<Status, std::string> MessageParser::ParseErrorAndStatus(boost::property_tree::ptree &pt) {
  auto statusString = pt.get<std::string>(JsonFields::Status);

  Status status;
  if (statusString == JsonFields::Success) {
    status = Status::Success;
  } else if (statusString == JsonFields::Fail) {
    status = Status::Fail;
  } else {
    throw boost::property_tree::ptree_error(statusString);
  }

  auto error = pt.get<std::string>(JsonFields::ErrorMessage);

  return {Status, error};
}

}  // namespace Network

