//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#pragma once
#include <iostream>
#include <Room.hpp>
#include <vector>

enum commandType {
    INIT_REQUEST, CONNECT_REQUEST,
    START_GAME_REQUEST, TEXT_REQUEST,
    STATUS_REQUEST, WRITTEN_TEXT_REQUEST,
    INIT_RESPONSE, CONNECT_RESPONSE,
    START_GAME_RESPONSE, TEXT_RESPONSE,
    STATUS_RESPONSE, WRITTEN_TEXT_RESPONSE};

enum controllerType {BASE, GAME};

enum roomStatus{WAIT, PLAY, END};

class Command {
 public:
  /*Command(const std::string &data, const std::string &connectionUUID):
      connectionUUID(connectionUUID),
      data(data) {}*/
  Command() {}
  virtual ~Command() {}
  /*const std::string &parseToJSON () {
    return data;
  }
  std::string connectionUUID;
  std::string data;*/
 protected:
    commandType typeOfCommand;
};

class parsableFromJSON{
 public:
    virtual void parseFromJSON(std::string data) = 0;
};

class ClientCommand: public Command, public parsableFromJSON{
 public:
    ClientCommand();
    ClientCommand(const std::string &clientUUID, const std::string &connectionUUID,
            controllerType typeOfController);
    virtual ~ClientCommand() {}
    virtual void parseFromJSON(std::string data);
 protected:
    std::string clientUUID;
    std::string connectionUUID;
    controllerType typeOfController;
};


class InitRequest: public ClientCommand{
 public:
    InitRequest();
    ~InitRequest() {}
    virtual void parseFromJSON(std::string data);

    std::string name;
};

class ConnectRequest: public ClientCommand{
 public:
    ConnectRequest();
    ~ConnectRequest() {}
    virtual void parseFromJSON(std::string data);

    std::string key;
};

class StartGameSessionRequest: public ClientCommand{
 public:
    StartGameSessionRequest();
    ~StartGameSessionRequest() {}
    virtual void parseFromJSON(std::string data);

    std::string key;
};

class GetText: public ClientCommand{
 public:
    GetText();
    ~GetText() {}
    virtual void parseFromJSON(std::string data);

    std::string key;
};

class GetRoomStatus: public ClientCommand{
 public:
    GetRoomStatus();
    ~GetRoomStatus() {}
    virtual void parseFromJSON(std::string data);

    std::string key;
};

class SendWrittenText: public ClientCommand{
 public:
    SendWrittenText();
    ~SendWrittenText() {}
    virtual void parseFromJSON(std::string data);

    std::string key;
    std::string writtenText;
};

class parsableToJSON{
 public:
    virtual std::string parseToJSON(std::string data) = 0;
};

class ServerCommand: public Command, public parsableToJSON{
 public:
    ServerCommand();
    ServerCommand(const std::string &clientUUID, bool success, const std::string errorMessage);
    virtual ~ServerCommand() {}
    virtual std::string parseToJSON(std::string data);
 protected:
    std::string clientUUID;
    bool success;
    std::string errorMsg;
};


class InitResponse: public ServerCommand{
 public:
    explicit InitResponse(const std::string &key);
    ~InitResponse() {}
    virtual std::string parseToJSON(std::string data);

    std::string key;
};

class ConnectResponse: public ServerCommand{
 public:
    explicit ConnectResponse(const std::string &key);
    ~ConnectResponse() {}
    virtual std::string parseToJSON(std::string data);

    std::string key;
};

class StartGameSessionResponse: public ServerCommand{
 public:
    explicit StartGameSessionResponse(const std::string &key);
    ~StartGameSessionResponse() {}
    virtual std::string parseToJSON(std::string data);

    std::string key;
};

class SetText: public ServerCommand{
 public:
    explicit SetText(const std::string &text);
    ~SetText() {}
    virtual std::string parseToJSON(std::string data);

    std::string text;
};

class SendRoomStatus: public ServerCommand{
 public:
    SendRoomStatus(roomStatus statusOfRoom, double timeFromStart, std::vector<Player> players);
    ~SendRoomStatus() {}
    virtual std::string parseToJSON(std::string data);

    roomStatus statusOfRoom;
    double timeFromStart;
    std::vector<Player> players;
};

class ValidateWrittenText: public ServerCommand{
 public:
    explicit ValidateWrittenText(const size_t &right_count);
    ~ValidateWrittenText() {}
    virtual std::string parseToJSON(std::string data);

    size_t right_count;
};
