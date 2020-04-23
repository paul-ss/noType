//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "Command.hpp"

ClientCommand::ClientCommand() {}
ClientCommand::ClientCommand(const std::string &clientUUID, const std::string &connectionUUID,
        controllerType typeOfController):clientUUID(clientUUID), connectionUUID(connectionUUID),
        typeOfController(typeOfController) {}
void ClientCommand::parseFromJSON(std::string data) {}

InitRequest::InitRequest() {}
void InitRequest::parseFromJSON(std::string data) {}

ConnectRequest::ConnectRequest() {}
void ConnectRequest::parseFromJSON(std::string data) {}

StartGameSessionRequest::StartGameSessionRequest() {}
void StartGameSessionRequest::parseFromJSON(std::string data) {}

GetText::GetText() {}
void GetText::parseFromJSON(std::string data) {}

GetRoomStatus::GetRoomStatus() {}
void GetRoomStatus::parseFromJSON(std::string data) {}

SendWrittenText::SendWrittenText() {}
void SendWrittenText::parseFromJSON(std::string data) {}

ServerCommand::ServerCommand() {}
ServerCommand::ServerCommand(const std::string &clientUUID, bool success, std::string errorMessage = ""):
clientUUID(clientUUID), success(success) { errorMsg = errorMessage; }
std::string ServerCommand::parseToJSON(std::string data) {
    return data;
}

InitResponse::InitResponse(const std::string &key): key(key) {}
std::string InitResponse::parseToJSON(std::string data) {
    return data;
}

ConnectResponse::ConnectResponse(const std::string &key): key(key) {}
std::string ConnectResponse::parseToJSON(std::string data) {
    return data;
}

StartGameSessionResponse::StartGameSessionResponse(const std::string &key): key(key) {}
std::string StartGameSessionResponse::parseToJSON(std::string data) {
    return data;
}

SetText::SetText(const std::string &text):text(text) {}
std::string SetText::parseToJSON(std::string data) {
    return data;
}

SendRoomStatus::SendRoomStatus(roomStatus statusOfRoom, double timeFromStart, std::vector<Player> players):
statusOfRoom(statusOfRoom), timeFromStart(timeFromStart), players(players) {}
std::string SendRoomStatus::parseToJSON(std::string data) {
    return data;
}

ValidateWrittenText::ValidateWrittenText(const size_t &right_count):right_count(right_count) {}
std::string ValidateWrittenText::parseToJSON(std::string data) {
    return data;
}
