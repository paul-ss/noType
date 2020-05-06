//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#pragma once
#include <iostream>
#include <Room.hpp>
#include <vector>
#include <Parse.hpp>

enum commandType {
    INIT_REQUEST, CONNECT_REQUEST,
    START_GAME_REQUEST, TEXT_REQUEST,
    ROOM_STATUS_REQUEST, WRITTEN_TEXT_REQUEST,
    INIT_RESPONSE, CONNECT_RESPONSE,
    START_GAME_RESPONSE, TEXT_RESPONSE,
    STATUS_RESPONSE, WRITTEN_TEXT_RESPONSE};

enum controllerType {BASE, GAME};

enum roomStatus{WAIT, PLAY, END};

class Command {
 public:
    explicit Command(commandType typeOfCommand);
    virtual ~Command() {}

    commandType getTypeOfCommand();
 protected:
    commandType typeOfCommand;
};



class ClientCommand: public Command, public parsableFromJSON{
 public:
    ClientCommand(const std::string &data, commandType typeOfCommand, controllerType typeOfController);
    virtual ~ClientCommand() {}
    int parseFromJSON(const std::string &data) override;
    std::string getClientUUID();
    std::string getConnectionUUID();
    controllerType getTypeOfController();
 protected:
    std::string clientUUID;
    std::string connectionUUID;
    controllerType typeOfController;
};


class InitRequest: public ClientCommand{
 public:
    InitRequest(const std::string &data,
                controllerType typeOfController, commandType typeOfCommand);
    ~InitRequest() {}
    int parseFromJSON(const std::string &data) override;
    std::string name;
};

class ConnectRequest: public ClientCommand{
 public:
    ConnectRequest(const std::string &data,
                   controllerType typeOfController, commandType typeOfCommand);
    ~ConnectRequest() {}
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class StartGameSessionRequest: public ClientCommand{
 public:
    StartGameSessionRequest(const std::string &data,
                            controllerType typeOfController, commandType typeOfCommand);
    ~StartGameSessionRequest() {}
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class GetText: public ClientCommand{
 public:
    GetText(const std::string &data,
            controllerType typeOfController, commandType typeOfCommand);
    ~GetText() {}
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class GetRoomStatus: public ClientCommand{
 public:
    GetRoomStatus(const std::string &data,
                  controllerType typeOfController, commandType typeOfCommand);
    ~GetRoomStatus() {}
    int parseFromJSON(const std::string &data) override;

    std::string key;
};

class SendWrittenText: public ClientCommand{
 public:
    SendWrittenText(const std::string &data,
                    controllerType typeOfController, commandType typeOfCommand);
    ~SendWrittenText() {}
    int parseFromJSON(const std::string &data) override;

    std::string key;
    std::string writtenText;
};


class ServerCommand: public Command, public parsableToJSON{
 public:
    // ServerCommand(commandType typeOfCommand);
    ServerCommand(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage);
    virtual ~ServerCommand() {}
    std::string parseToJSON() override = 0;
 protected:
    std::string clientUUID;
    bool success;
    std::string errorMsg;
};


class InitResponse: public ServerCommand{
 public:
    explicit InitResponse(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage, const std::string &keys);
    ~InitResponse() {}
    std::string parseToJSON() override;
    std::string key;
};

class ConnectResponse: public ServerCommand{
 public:
    explicit ConnectResponse(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage, const std::string &keys);
    ~ConnectResponse() {}
    std::string parseToJSON() override;

    std::string key;
};

class StartGameSessionResponse: public ServerCommand{
 public:
    explicit StartGameSessionResponse(commandType typeOfCommand, const std::string &clientUUID,
            bool success, const std::string &errorMessage, const std::string &keys);
    ~StartGameSessionResponse() {}
    std::string parseToJSON() override;

    std::string key;
};

class SetText: public ServerCommand{
 public:
    explicit SetText(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage, const std::string &text);
    ~SetText() {}
    std::string parseToJSON() override;

    std::string text;
};

class SendRoomStatus: public ServerCommand{
 public:
    SendRoomStatus(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage, roomStatus statusOfRoom, double timeFromStart,
            std::vector<Player> players);
    ~SendRoomStatus() {}
    std::string parseToJSON() override;

    roomStatus statusOfRoom;
    double timeFromStart;
    std::vector<Player> players;
};

class ValidateWrittenText: public ServerCommand{
 public:
    explicit ValidateWrittenText(commandType typeOfCommand, const std::string &clientUUID, bool success,
            const std::string &errorMessage, const size_t &right_count);
    ~ValidateWrittenText() {}
    std::string parseToJSON() override;

    size_t right_count;
};
