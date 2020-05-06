//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "Command.hpp"
#define SUCCESS 0
#define FAIL 1

Command::Command(commandType typeOfCommand): typeOfCommand(typeOfCommand) {}

commandType Command::getTypeOfCommand() {
    return typeOfCommand;
}

ClientCommand::ClientCommand(const std::string &data, commandType typeOfCommand,
        controllerType typeOfController):
        Command(typeOfCommand), typeOfController(typeOfController) {ClientCommand::parseFromJSON(data);}
std::string ClientCommand::getClientUUID() {
    return clientUUID;
}
std::string ClientCommand::getConnectionUUID() {
    return connectionUUID;
}
controllerType ClientCommand::getTypeOfController() {
    return typeOfController;
}

int ClientCommand::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("clientUUID"));
    assert(doc["clientUUID"].IsString());
    clientUUID = doc["clientUUID"].GetString();
    assert(doc.HasMember("connectionUUID"));
    assert(doc["connectionUUID"].IsString());
    connectionUUID = doc["connectionUUID"].GetString();
    return SUCCESS;
}

InitRequest::InitRequest(const std::string &data,
                         controllerType typeOfController, commandType typeOfCommand):
        ClientCommand(data, typeOfCommand, typeOfController) {InitRequest::parseFromJSON(data);}
int InitRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("name"));
    assert(doc["name"].IsString());
    name = doc["name"].GetString();
    return SUCCESS;
}

ConnectRequest::ConnectRequest(const std::string &data,
                               controllerType typeOfController, commandType typeOfCommand):
        ClientCommand(data, typeOfCommand, typeOfController) {ConnectRequest::parseFromJSON(data);}
int ConnectRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

StartGameSessionRequest::StartGameSessionRequest(const std::string &data, controllerType typeOfController,
        commandType typeOfCommand): ClientCommand(data, typeOfCommand, typeOfController)
        {StartGameSessionRequest::parseFromJSON(data);}
int StartGameSessionRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

GetText::GetText(const std::string &data, controllerType typeOfController, commandType typeOfCommand):
        ClientCommand(data, typeOfCommand, typeOfController) {GetText::parseFromJSON(data);}
int GetText::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

GetRoomStatus::GetRoomStatus(const std::string &data, controllerType typeOfController,
        commandType typeOfCommand):
        ClientCommand(data, typeOfCommand, typeOfController) {GetRoomStatus::parseFromJSON(data);}
int GetRoomStatus::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

SendWrittenText::SendWrittenText(const std::string &data, controllerType typeOfController,
        commandType typeOfCommand):
        ClientCommand(data, typeOfCommand, typeOfController) {}
int SendWrittenText::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    assert(doc.HasMember("writtenText"));
    assert(doc["writtenText"].IsString());
    writtenText = doc["writtenText"].GetString();
    return SUCCESS;
}


ServerCommand::ServerCommand(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage): Command(typeOfCommand), clientUUID(clientUUID), success(success),
        errorMsg(errorMessage) {}



InitResponse::InitResponse(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage, const std::string &keys):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage), key(keys) {}
std::string InitResponse::parseToJSON() {
    std::string data;
    return data;
}

ConnectResponse::ConnectResponse(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage, const std::string &keys):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage), key(keys) {}
std::string ConnectResponse::parseToJSON() {
    std::string data;
    return data;
}

StartGameSessionResponse::StartGameSessionResponse(commandType typeOfCommand, const std::string &clientUUID,
        bool success, const std::string &errorMessage, const std::string &keys):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage), key(keys) {}
std::string StartGameSessionResponse::parseToJSON() {
    std::string data;
    return data;
}

SetText::SetText(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage, const std::string &text):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage), text(text) {}
std::string SetText::parseToJSON() {
    std::string data;
    return data;
}

SendRoomStatus::SendRoomStatus(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage, roomStatus statusOfRoom, double timeFromStart,
        std::vector<Player> players):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage),
        statusOfRoom(statusOfRoom), timeFromStart(timeFromStart), players(players) {}
std::string SendRoomStatus::parseToJSON() {
    std::string data;
    return data;
}

ValidateWrittenText::ValidateWrittenText(commandType typeOfCommand, const std::string &clientUUID, bool success,
        const std::string &errorMessage, const size_t &right_count):
        ServerCommand(typeOfCommand, clientUUID, success, errorMessage), right_count(right_count) {}
std::string ValidateWrittenText::parseToJSON() {
    std::string data;
    return data;
}
