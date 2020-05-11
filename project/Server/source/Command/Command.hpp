//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#pragma once

#include "IRoomStatus.hpp"

#include <iostream>
#include <vector>
#include <Parse.hpp>
#include <unordered_map>


enum commandType {
    INIT_REQUEST, INIT_RESPONSE,
    CONNECT_REQUEST, CONNECT_RESPONSE,
    START_GAME_SESSION_REQUEST, START_GAME_SESSION_RESPONSE,
    GET_TEXT_REQUEST, GET_TEXT_RESPONSE,
    ROOM_STATUS_REQUEST, ROOM_STATUS_RESPONSE,
    VALIDATE_WRITTEN_TEXT_REQUEST, VALIDATE_WRITTEN_TEXT_RESPONSE,
    ERROR};

enum controllerType {BASE, GAME};

//enum roomState {WAIT, PLAY, END};

enum status {success, fail};

class Command {
 public:
    explicit Command(const commandType &typeOfCommand);
    Command() = default;
    virtual ~Command() = default;

    commandType getTypeOfCommand();
 protected:
    commandType typeOfCommand;
};


class ClientCommand: public Command, public parsableFromJSON{
 public:
    ClientCommand() = default;
    ClientCommand(const std::string &data,
                  const commandType &typeOfCommand,
                  const controllerType &typeOfController);
    virtual ~ClientCommand() = default;
    int parseFromJSON(const std::string &data) override;
    std::string getConnectionUUID();
    controllerType getTypeOfController();

 public:
    std::string connectionUUID;
    controllerType typeOfController;
};


class InitRequest: public ClientCommand{
 public:
    InitRequest(const std::string &data,
                const commandType &typeOfCommand,
                const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;
    std::string name;
};

class ConnectRequest: public ClientCommand{
 public:
    ConnectRequest(const std::string &data,
                   const commandType &typeOfCommand,
                   const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class StartGameSessionRequest: public ClientCommand{
 public:
    StartGameSessionRequest(const std::string &data,
                            const commandType &typeOfCommand,
                            const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class GetTextRequest: public ClientCommand{
 public:
    GetTextRequest(const std::string &data,
                   const commandType &typeOfCommand,
                   const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class RoomStatusRequest: public ClientCommand{
 public:
    RoomStatusRequest(const std::string &data,
                      const commandType &typeOfCommand,
                      const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class ValidateWrittenTextRequest: public ClientCommand{
 public:
    ValidateWrittenTextRequest(const std::string &data,
                               const commandType &typeOfCommand,
                               const controllerType &typeOfController);
    int parseFromJSON(const std::string &data) override;

    std::string key;
    std::string writtenText;
};


class ServerCommand: public Command, public parsableToJSON{
 public:
    ServerCommand(commandType typeOfCommand, const std::string &clientUUID, status state,
            const std::string &errorMessage);
    virtual ~ServerCommand() = default;
    std::string parseToJSON() override;

 public:
    std::string clientUUID;
    status state;
    std::string errorMsg;
};


class InitResponse: public ServerCommand{
 public:
    InitResponse(const std::string &clientUUID, const std::string &keys);
    InitResponse(const std::string &clientUUID , status state, const std::string &errorMessage);

    std::string parseToJSON() override;
    std::string key;
};



class ConnectResponse: public ServerCommand{
 public:
    explicit ConnectResponse(const std::string &clientUUID);
    ConnectResponse(const std::string &clientUUID, status state, const std::string &errorMessage);
};

class StartGameSessionResponse: public ServerCommand{
 public:
    StartGameSessionResponse(const std::string &clientUUID, const std::string &playID, double waitTime);
    StartGameSessionResponse(const std::string &clientUUID,
                             status state, const std::string &errorMessage);
    std::string parseToJSON() override;

    std::string playerID;
    double waitTime;
};

class GetTextResponse: public ServerCommand{
 public:
    GetTextResponse(const std::string &clientUUID, const std::string &text);
    GetTextResponse(const std::string &clientUUID, status state,
                             const std::string &errorMessage);
    std::string parseToJSON() override;

    std::string text;
};

class RoomStatusResponse: public ServerCommand{
 public:
    RoomStatusResponse(const std::string &clientUUID,
                        const RoomState &statusOfRoom, double timeFromStart,
                       std::unordered_map<std::string, Player> &players);
    RoomStatusResponse(const std::string &clientUUID, status state,
                       const std::string &errorMessage);
    std::string parseToJSON() override;

    RoomState statusOfRoom;
    double timeFromStart;
    std::unordered_map<std::string, Player> players;
};

class ValidateWrittenTextResponse: public ServerCommand{
 public:
    ValidateWrittenTextResponse(const std::string &clientUUID, const size_t &right_count);
    ValidateWrittenTextResponse(const std::string &clientUUID,
                                         status state, const std::string &errorMessage);
    std::string parseToJSON() override;

    size_t right_count;
};

class Error: public ServerCommand{
 public:
    Error(const std::string &clientUUID, const std::string &errorMessage);
};
