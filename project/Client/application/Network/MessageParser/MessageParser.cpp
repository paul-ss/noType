#include "MessageParserConfig.hpp"
#include "MessageParser.hpp"
#include "Message.hpp"
#include "logger.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/exceptions.hpp>

namespace Network {

std::string MessageParser::ParseToJson(std::unique_ptr<Message> msg) {
  std::string res;
  try {
    switch (msg->GetMessageType()) {
      case MessageType::InitRequest: {
        res = initRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ConnectRequest: {
        res = connectRequestToJson(std::move(msg));
        break;
      }
      case MessageType::StartGameSessionRequest:{
        res = startGameSessionRequestToJson(std::move(msg));
        break;
      }
      case MessageType::GetTextRequest: {
        res = getTextRequestToJson(std::move(msg));
        break;
      }
      case MessageType::RoomStatusRequest: {
        res = roomStatusRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ValidateWrittenTextRequest: {
        res = validateWrittenTextRequestToJson(std::move(msg));
        break;
      }
      case MessageType::ErrorRequest: {
        res = errorRequestToJson(std::move(msg));
        break;
      } case MessageType::LeaveRoomRequest: {
        res = leaveRoomRequestToJson(std::move(msg));
        break;
      }
      default: {
        BOOST_LOG_TRIVIAL(error) << "MessageParser::ParseToJson: Invalid MessageType";
        break;
      }
    }
  } catch (const boost::property_tree::ptree_error& error) {
    BOOST_LOG_TRIVIAL(error) << "MessageParser::ParseToJson: " << error.what();
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
      return initResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ConnectResponse) {
      return connectResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::StartGameSessionResponse) {
      return startGameSessionResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::GetTextResponse) {
      return getTextResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::RoomStatusResponse) {
      return roomStatusResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ValidateWrittenTextResponse) {
      return validateWrittenTextResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::ErrorResponse) {
      return errorResponseToMessage(pt);
    } else if (stringMessageType == StringMessageType::LeaveRoomResponse) {
      return leaveRoomResponseToMessage(pt);
    }
  } catch (const boost::property_tree::ptree_error& error) {
    BOOST_LOG_TRIVIAL(error) << "MessageParser::ParseToMessage: " << error.what();;
  }
  
  return nullptr;
}

std::string MessageParser::initRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto initRequestData = std::any_cast<InitRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::InitRequest);
  pt.put(JsonFields::Name, initRequestData.name);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::connectRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto connectRequestData = std::any_cast<ConnectRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::ConnectRequest);
  pt.put(JsonFields::ClientUuid, connectRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::startGameSessionRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto startGameSessionRequestData = std::any_cast<StartGameSessionRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::StartGameSessionRequest);
  pt.put(JsonFields::ClientUuid, startGameSessionRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::roomStatusRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto roomStatusRequestData = std::any_cast<RoomStatusRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::RoomStatusRequest);
  pt.put(JsonFields::ClientUuid, roomStatusRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::getTextRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto getTextRequestData = std::any_cast<GetTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::GetTextRequest);
  pt.put(JsonFields::ClientUuid, getTextRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::validateWrittenTextRequestToJson(std::unique_ptr<Message> msg) {
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

std::string MessageParser::leaveRoomRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto leaveRoomRequest = std::any_cast<LeaveRoomRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::CommandType, StringMessageType::LeaveRoomRequest);
  pt.put(JsonFields::ClientUuid, leaveRoomRequest.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

std::string MessageParser::errorRequestToJson(std::unique_ptr<Message> msg) {
  auto anyData = msg->ExtractData();
  auto validateWrittenTextRequestData = std::any_cast<ValidateWrittenTextRequest>(anyData);

  boost::property_tree::ptree pt;
  pt.put(JsonFields::ClientUuid, validateWrittenTextRequestData.id);

  std::stringstream oss;
  boost::property_tree::write_json(oss, pt);

  return oss.str();
}

// =====================================================================================================================

std::unique_ptr<Message> MessageParser::initResponseToMessage(boost::property_tree::ptree &pt) {
  auto id = pt.get<std::string>(JsonFields::ClientUuid);
  auto errorData = parseErrorAndStatus(pt);

  InitResponse initResponse = {id, errorData.first, errorData.second};
  std::any data = initResponse;
  return std::make_unique<Message>(MessageType::InitResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::connectResponseToMessage(boost::property_tree::ptree &pt) {
  auto errorData = parseErrorAndStatus(pt);

  ConnectResponse connectResponse = {errorData.first, errorData.second};
  std::any data = connectResponse;
  return std::make_unique<Message>(MessageType::ConnectResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::startGameSessionResponseToMessage(boost::property_tree::ptree &pt) {
  auto playerId = pt.get<std::string>(JsonFields::PlayerId);
  auto waitTime = pt.get<size_t>(JsonFields::WaitTime);
  auto errorData = parseErrorAndStatus(pt);

  StartGameSessionResponse startGameSessionResponse = {playerId, waitTime, errorData.first, errorData.second};
  std::any data = startGameSessionResponse;
  return std::make_unique<Message>(MessageType::StartGameSessionResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::getTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto text = pt.get<std::string>(JsonFields::Text);
  auto errorData = parseErrorAndStatus(pt);

  GetTextResponse getTextResponse = {text, errorData.first, errorData.second};
  std::any data = getTextResponse;
  return std::make_unique<Message>(MessageType::GetTextResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::roomStatusResponseToMessage(boost::property_tree::ptree& pt) {
  auto errorData = parseErrorAndStatus(pt);

  auto stringPlayerStatus = pt.get<std::string>(JsonFields::Room::Status);

  RoomStatus roomStatus = parseRoomStatus(stringPlayerStatus);

  auto playersInfo = parsePlayersInfo(pt);

  RoomStatusResponse roomStatusResponse = {playersInfo, roomStatus, errorData.first, errorData.second};
  std::any data = roomStatusResponse;
  return std::make_unique<Message>(MessageType::RoomStatusResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::validateWrittenTextResponseToMessage(boost::property_tree::ptree& pt) {
  auto rightCount = pt.get<std::uint16_t>(JsonFields::RightCount);
  auto errorData = parseErrorAndStatus(pt);

  ValidateWrittenTextResponse validateWrittenTextResponse = {rightCount, errorData.first, errorData.second};
  std::any data = validateWrittenTextResponse;
  return std::make_unique<Message>(MessageType::ValidateWrittenTextResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::leaveRoomResponseToMessage(boost::property_tree::ptree& pt) {
  auto errorData = parseErrorAndStatus(pt);

  LeaveRoomResponse leaveRoomResponse = {errorData.first, errorData.second};
  std::any data = leaveRoomResponse;
  return std::make_unique<Message>(MessageType::LeaveRoomResponse, std::move(data));
}

std::unique_ptr<Message> MessageParser::errorResponseToMessage(boost::property_tree::ptree& pt) {
  auto error = pt.get<std::string>(JsonFields::ErrorMessage);

  ErrorResponse errorResponse = {error};

  std::any data = errorResponse;
  return std::make_unique<Message>(MessageType::ErrorResponse, std::move(data));
}

std::pair<Status, std::string> MessageParser::parseErrorAndStatus(boost::property_tree::ptree &pt) {
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

RoomStatus MessageParser::parseRoomStatus(const std::string& stringPlayerStatus) {
  if (stringPlayerStatus == JsonFields::Room::Wait) {
    return RoomStatus::Wait;
  } else if (stringPlayerStatus == JsonFields::Room::Play) {
    return RoomStatus::Play;
  } else if (stringPlayerStatus == JsonFields::Room::End) {
    return RoomStatus::End;
  }

  return RoomStatus::End;
}

static PlayerInfo::Status parsePlayerStatus(const std::string& stringPlayerStatus) {
  if (stringPlayerStatus == JsonFields::PlayerInfoFields::Play) {
    return PlayerInfo::Status::Play;
  } else if (stringPlayerStatus == JsonFields::PlayerInfoFields::Finish) {
    return PlayerInfo::Status::Finish;
  } else if (stringPlayerStatus == JsonFields::PlayerInfoFields::Win) {
    return PlayerInfo::Status::Win;
  }

  return PlayerInfo::Status::Win;
}

std::unordered_map<std::string, PlayerInfo> MessageParser::parsePlayersInfo(boost::property_tree::ptree& pt) {
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
    playerInfo.status = parsePlayerStatus(stringPlayerStatus);

    playersInfo[uuid] = playerInfo;
  }

  return playersInfo;
}


}  // namespace Network

