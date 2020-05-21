#include "MessageParserConfig.hpp"
#include "MessageParser.hpp"
#include "Message.hpp"

#include <iostream>

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
  try {
    switch (msg->GetMessageType()) {
      case MessageType::InitRequest: {
        res = InitRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ConnectRequest: {
        res = ConnectRequestToJson(std::move(msg));
        break;
      }
      case MessageType::StartGameSessionRequest:{
        res = StartGameSessionRequestToJson(std::move(msg));
        break;
      }
      case MessageType::GetTextRequest: {
        res = GetTextRequestToJson(std::move(msg));
        break;
      }
      case MessageType::RoomStatusRequest: {
        res = RoomStatusRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ValidateWrittenTextRequest: {
        res = ValidateWrittenTextRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ErrorRequest: {
        res = ErrorRequestToJson(std::move(msg));
        break;
      }
      default: {
        std::cerr << "Invalid request type" << std::endl;
        break;
      }
    }
  } catch (const boost::property_tree::ptree_error& error) {
    std::cout << "[MessageParser::ParseToJson]: " << error.what() << std::endl;
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
      return ConnectResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::StartGameSessionResponse) {
      return StartGameSessionResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::GetTextResponse) {
      return GetTextResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::RoomStatusResponse) {
      return RoomStatusResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ValidateWrittenTextResponse) {
      return ValidateWrittenTextResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ErrorResponse) {
      return ErrorResponseToMessage(pt);
    }
  } catch (const boost::property_tree::ptree_error& error) {
    std::cout << "[MessageParser::ParseToMessage]: " << error.what() << std::endl;
  }
  
  return nullptr;
}

std::string MessageParser::InitRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto initRequestData = std::any_cast<InitRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::InitRequest);
  pt.put(JsonFields::Name, initRequestData.name);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::ConnectRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto connectRequestData = std::any_cast<ConnectRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::ConnectRequest);
  pt.put(JsonFields::ClientUuid, connectRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::StartGameSessionRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto startGameSessionRequestData = std::any_cast<StartGameSessionRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::StartGameSessionRequest);
  pt.put(JsonFields::ClientUuid, startGameSessionRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::RoomStatusRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto roomStatusRequestData = std::any_cast<RoomStatusRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::RoomStatusRequest);
  pt.put(JsonFields::ClientUuid, roomStatusRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::GetTextRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto getTextRequestData = std::any_cast<GetTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::GetTextRequest);
  pt.put(JsonFields::ClientUuid, getTextRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::ValidateWrittenTextRequestToJson(std::unique_ptr<Message> msg) {
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

std::string MessageParser::ErrorRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto validateWrittenTextRequestData = std::any_cast<ValidateWrittenTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::ClientUuid, validateWrittenTextRequestData.id);

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
  return std::make_unique<Message>(MessageType::InitResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::ConnectResponseToMessage(boost::property_tree::ptree &pt) {
  auto errorData = ParseErrorAndStatus(pt);

  ConnectResponse connectResponse = {errorData.first, errorData.second};
  std::any data = connectResponse;
  return std::make_unique<Message>(MessageType::ConnectResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::StartGameSessionResponseToMessage(boost::property_tree::ptree &pt) {
  auto id = pt.get<std::string>(JsonFields::ClientUuid);
  auto waitTime = pt.get<size_t>(JsonFields::WaitTime);
  auto errorData = ParseErrorAndStatus(pt);

  StartGameSessionResponse startGameSessionResponse = {id, waitTime, errorData.first, errorData.second};
  std::any data = startGameSessionResponse;
  return std::make_unique<Message>(MessageType::StartGameSessionResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::GetTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto text = pt.get<std::string>(JsonFields::Text);
  auto errorData = ParseErrorAndStatus(pt);

  GetTextResponse getTextResponse = {text, errorData.first, errorData.second};
  std::any data = getTextResponse;
  return std::make_unique<Message>(MessageType::GetTextResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::RoomStatusResponseToMessage(boost::property_tree::ptree& pt) {
  auto timeFromStart = pt.get<double>(JsonFields::Text);
  auto errorData = ParseErrorAndStatus(pt);

  auto stringPlayerStatus = pt.get<std::string>(JsonFields::Room::Status);

  RoomStatus roomStatus;
  if (stringPlayerStatus == JsonFields::Room::Wait) {
    roomStatus = RoomStatus::Wait;
  } else if (stringPlayerStatus == JsonFields::Room::Play) {
    roomStatus = RoomStatus::Play;
  } else if (stringPlayerStatus == JsonFields::Room::End) {
    roomStatus = RoomStatus::End;
  } else {
    // exeption
  }
  const std::string kComma = ".";

  std::unordered_map<std::string, PlayerInfo> playersInfo;
  for (const auto& [uuid, _]: pt.get_child(JsonFields::Players)) {
    PlayerInfo playerInfo;

    std::string pathToUuid = JsonFields::Players + kComma;
    pathToUuid += uuid;

    std::string pathToName = pathToUuid + kComma;
    pathToName += JsonFields::PlayerInfoFields::Name;

    playerInfo.name = pt.get<std::string>(pathToName);

    std::string pathToPosition = pathToUuid + kComma;
    pathToPosition += JsonFields::PlayerInfoFields::Position;

    playerInfo.position = pt.get<std::uint16_t>(pathToPosition);

    std::string pathToSpeed = pathToUuid + kComma;
    pathToSpeed += JsonFields::PlayerInfoFields::Speed;

    playerInfo.position = pt.get<double>(pathToSpeed);

    std::string pathToStatus = pathToUuid + kComma;
    pathToStatus += JsonFields::PlayerInfoFields::Status;

    auto stringPlayerStatus = pt.get<std::string>(pathToStatus);
    if (stringPlayerStatus == JsonFields::PlayerInfoFields::Play) {
      playerInfo.status = PlayerInfo::Status::Play;
    } else if (stringPlayerStatus == JsonFields::PlayerInfoFields::Finish) {
      playerInfo.status = PlayerInfo::Status::Finish;
    } else if (stringPlayerStatus == JsonFields::PlayerInfoFields::Win) {
      playerInfo.status = PlayerInfo::Status::Win;
    }

    playersInfo[uuid] = playerInfo;
  }

  RoomStatusResponse roomStatusResponse = {timeFromStart, playersInfo, roomStatus, errorData.first, errorData.second};
  std::any data = roomStatusResponse;
  return std::make_unique<Message>(MessageType::RoomStatusResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::ValidateWrittenTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto rightCount = pt.get<std::uint16_t>(JsonFields::RightCount);
  auto errorData = ParseErrorAndStatus(pt);

  ValidateWrittenTextResponse validateWrittenTextResponse = {rightCount, errorData.first, errorData.second};
  std::any data = validateWrittenTextResponse;
  return std::make_unique<Message>(MessageType::ValidateWrittenTextResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::ErrorResponseToMessage(boost::property_tree::ptree& pt) {
  auto error = pt.get<std::string>(JsonFields::ErrorMessage);

  ErrorResponse errorResponse = {error};

  std::any data = errorResponse;
  return std::make_unique<Message>(MessageType::ErrorResponse, std::move(data));
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

  return {status, error};
}

}  // namespace Network

