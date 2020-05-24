//
// Created by paul_s on 17.04.2020.
//

#pragma once

#include "IRoomStatus.hpp"
#include "Logger.hpp"

#include <iostream>
#include <vector>
#include <Parse.hpp>
#include <unordered_map>

#include <enum.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#define COMMAND_TYPE_JSON_PATH "commandType"
#define CLIENT_UUID_JSON_PATH "clientUUID"
#define RESPONSE_STATUS_JSON_PATH "status"
#define ERROR_MSG_JSON_PATH "errorMsg"
#define PLAYER_ID_JSON_PATH "playerID"
#define WAIT_TIME_JSON_PATH "waitTime"
#define ROOM_TEXT_JSON_PATH "text"
#define ROOM_STATUS_JSON_PATH "roomStatus"
#define ROOM_PLAYERS_JSON_PATH "players"
#define WRITTEN_TEXT_JSON_PATH "writtenText"
#define RIGHT_COUNT_JSON_PATH "rightCount"
#define CLIENT_NAME_JSON_PATH "clientName"

#define PLAYER_NAME_JSON_DIR "name"
#define PLAYER_POS_JSON_DIR "position"
#define PLAYER_SPEED_JSON_DIR "speed"
#define PLAYER_STATUS_JSON_DIR "status"

namespace pt = boost::property_tree;



BETTER_ENUM (CommandType, int,
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
    LeaveRoomRequest,
    LeaveRoomResponse,
    ErrorRequest,
    ErrorResponse
)



BETTER_ENUM (ResponseStatus, int, success, fail)

enum class ControllerType {basic, game};




class Command {
 public:
    Command(CommandType commandType, const std::string &connectionUUID);
    Command();
    virtual ~Command() = default;

    CommandType getCommandType();
    std::string getConnectionUUID();

protected:
  CommandType _commandType;
  std::string _connectionUUID;
};




class ClientCommand: public Command {
 public:
    ClientCommand();
    ClientCommand(CommandType commandType, const std::string &connectionUUID);
    ControllerType getControllerType();

    virtual void parseFromPtree(pt::ptree &&pTree) = 0;

 private:
    ControllerType controllerTypeFromCommandType(CommandType commandType);

 protected:
    ControllerType _controllerType;
};




class ServerCommand: public Command {
public:
  ServerCommand(CommandType commandType, const std::string &connectionUUID);
  void setError(std::string &&errorMsg);

  virtual std::string parseToJSON() = 0;

public:
  ResponseStatus _status;
  std::string _errorMsg;
};






class ErrorResponse: public ServerCommand {
 public:
    ErrorResponse(const std::string &connectionUUID, std::string &&errorMsg);
    explicit ErrorResponse(const std::string &connectionUUID);
    std::string parseToJSON() override ;
};



class ErrorRequest: public ClientCommand {
public:
  explicit ErrorRequest(const std::string &connectionUUID, const std::string &errorMsg);
  void parseFromPtree(pt::ptree &&pTree) override;
  std::string getErrorMsg();

private:
  std::string _errorMsg;
};