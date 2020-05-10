//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "Command.hpp"
#define SUCCESS 0
#define FAIL 1

Command::Command(const commandType &typeOfCommand): typeOfCommand(typeOfCommand) {}

commandType Command::getTypeOfCommand() {
    return typeOfCommand;
}

ClientCommand::ClientCommand(const std::string &data, const commandType &typeOfCommand,
                             const controllerType &typeOfController):
                             Command(typeOfCommand), typeOfController(typeOfController)
                             {ClientCommand::parseFromJSON(data);}

std::string ClientCommand::getConnectionUUID() {
    return connectionUUID;
}
controllerType ClientCommand::getTypeOfController() {
    return typeOfController;
}

int ClientCommand::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("connectionUUID"));
    assert(doc["connectionUUID"].IsString());
    connectionUUID = doc["connectionUUID"].GetString();
    return SUCCESS;
}

InitRequest::InitRequest(const std::string &data,
                         const commandType &typeOfCommand,
                         const controllerType &typeOfController):
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
                               const commandType &typeOfCommand,
                               const controllerType &typeOfController):
        ClientCommand(data, typeOfCommand, typeOfController) {ConnectRequest::parseFromJSON(data);}
int ConnectRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

StartGameSessionRequest::StartGameSessionRequest(const std::string &data, const commandType &typeOfCommand,
                                                 const controllerType &typeOfController):
                                                 ClientCommand(data, typeOfCommand, typeOfController)
                                                 {StartGameSessionRequest::parseFromJSON(data);}
int StartGameSessionRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

GetTextRequest::GetTextRequest(const std::string &data, const commandType &typeOfCommand,
                               const controllerType &typeOfController):
                               ClientCommand(data, typeOfCommand, typeOfController)
                               {GetTextRequest::parseFromJSON(data);}
int GetTextRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

RoomStatusRequest::RoomStatusRequest(const std::string &data, const commandType &typeOfCommand,
                                     const controllerType &typeOfController):
                                     ClientCommand(data, typeOfCommand, typeOfController)
                                     {RoomStatusRequest::parseFromJSON(data);}
int RoomStatusRequest::parseFromJSON(const std::string &data) {
    rapidjson::Document doc;
    doc.Parse(data.c_str());
    assert(doc.HasMember("key"));
    assert(doc["key"].IsString());
    key = doc["key"].GetString();
    return SUCCESS;
}

ValidateWrittenTextRequest::ValidateWrittenTextRequest(const std::string &data,
                                                        const commandType &typeOfCommand,
                                                       const controllerType &typeOfController):
                                                       ClientCommand(data, typeOfCommand, typeOfController)
                                                       {}
int ValidateWrittenTextRequest::parseFromJSON(const std::string &data) {
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


ServerCommand::ServerCommand(commandType typeOfCommand, const std::string &clientUUID, status state,
        const std::string &errorMessage): Command(typeOfCommand), clientUUID(clientUUID), state(state),
        errorMsg(errorMessage) {}

std::string ServerCommand::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    json_val.SetInt64(typeOfCommand);
    doc.AddMember("commandType", json_val, allocator);

    json_val.SetString(clientUUID.c_str(), allocator);
    doc.AddMember("clientUUID", json_val, allocator);

    json_val.SetString(errorMsg.c_str(), allocator);
    doc.AddMember("errorMessage", json_val, allocator);

    json_val.SetInt64(state);
    doc.AddMember("status", json_val, allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    return data;
}


InitResponse::InitResponse(const std::string &clientUUID, const std::string &keys):
            ServerCommand(INIT_RESPONSE, clientUUID, success, ""), key(keys) {}
InitResponse::InitResponse(const std::string &clientUUID, status state, const std::string &errorMessage):
            ServerCommand(INIT_RESPONSE, clientUUID, state, errorMessage), key("") {}
std::string InitResponse::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();

    json_val.SetString(key.c_str(), allocator);
    doc.AddMember("key", json_val, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    std::string dataFrom = ServerCommand::parseToJSON();
    dataFrom.erase(dataFrom.find('}'), 1);
    data.replace(data.find('{'),1,",");
    std::string result = dataFrom + data;
    return result;
}


ConnectResponse::ConnectResponse(const std::string &clientUUID):
                ServerCommand(CONNECT_RESPONSE, clientUUID, success, "") {}
ConnectResponse::ConnectResponse(const std::string &clientUUID, status state,
                const std::string &errorMessage):
                ServerCommand(CONNECT_RESPONSE, clientUUID, state, errorMessage) {}


StartGameSessionResponse::StartGameSessionResponse(const std::string &clientUUID,
                        const std::string &playID, double waitTime):
                        ServerCommand(START_GAME_SESSION_RESPONSE, clientUUID, success, ""),
                        playerID(playID), waitTime(waitTime) {}
StartGameSessionResponse::StartGameSessionResponse(const std::string &clientUUID,
                         status state, const std::string &errorMessage):
                        ServerCommand(START_GAME_SESSION_RESPONSE, clientUUID, state, errorMessage),
                        playerID(""), waitTime(0.0) {}
std::string StartGameSessionResponse::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();

    json_val.SetString(playerID.c_str(), allocator);
    doc.AddMember("playerID", json_val, allocator);

    json_val.SetDouble(waitTime);
    doc.AddMember("waitTime", json_val, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    std::string dataFrom = ServerCommand::parseToJSON();
    dataFrom.erase(dataFrom.find('}'), 1);
    data.replace(data.find('{'),1,",");
    std::string result = dataFrom + data;
    return result;
}

GetTextResponse::GetTextResponse(const std::string &clientUUID, const std::string &text):
        ServerCommand(GET_TEXT_RESPONSE, clientUUID, success, ""), text(text) {}
GetTextResponse::GetTextResponse(const std::string &clientUUID, status state,
                const std::string &errorMessage):
        ServerCommand(GET_TEXT_RESPONSE, clientUUID, state, errorMessage), text("") {}
std::string GetTextResponse::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();

    json_val.SetString(text.c_str(), allocator);
    doc.AddMember("text", json_val, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    std::string dataFrom = ServerCommand::parseToJSON();
    dataFrom.erase(dataFrom.find('}'), 1);
    data.replace(data.find('{'),1,",");
    std::string result = dataFrom + data;
    return result;
}


RoomStatusResponse::RoomStatusResponse(const std::string &clientUUID,
                   const roomStatus &statusOfRoom, double timeFromStart,
                   std::map<std::string, Player> &players):
        ServerCommand(ROOM_STATUS_RESPONSE, clientUUID, success, ""),
        statusOfRoom(statusOfRoom), timeFromStart(timeFromStart), players(players) {}
RoomStatusResponse::RoomStatusResponse(const std::string &clientUUID, status state,
                   const std::string &errorMessage):
        ServerCommand(ROOM_STATUS_RESPONSE, clientUUID, state, errorMessage),
        timeFromStart(0.0) {}
std::string RoomStatusResponse::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();
    json_val.SetDouble(timeFromStart);
    doc.AddMember("timeFromStart", json_val, allocator);
    rapidjson::Value obj_player;
    obj_player.SetObject();

    /*const std::string clientUUID;
    const size_t playerID;
    const std::string name;
    size_t textPosition;
    PlayerState state;

    size_t lastTextPosition;
    double currentSpeed;*/

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    std::string dataFrom = ServerCommand::parseToJSON();
    // dataFrom.erase(dataFrom.find('}'), 1);
    // data.replace(data.find('{'),1,",");
    std::string result = dataFrom + data;
    return result;
}

ValidateWrittenTextResponse::ValidateWrittenTextResponse(const std::string &clientUUID,
        const size_t &right_count):
        ServerCommand(VALIDATE_WRITTEN_TEXT_RESPONSE, clientUUID, success, ""),
        right_count(right_count) {}
ValidateWrittenTextResponse::ValidateWrittenTextResponse(const std::string &clientUUID,
                            status state, const std::string &errorMessage):
        ServerCommand(VALIDATE_WRITTEN_TEXT_RESPONSE, clientUUID, state, errorMessage), right_count(0) {}
std::string ValidateWrittenTextResponse::parseToJSON() {
    rapidjson::Value json_val;
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    doc.SetObject();

    json_val.SetUint64(right_count);
    doc.AddMember("right_count", json_val, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string data = buffer.GetString();
    std::string dataFrom = ServerCommand::parseToJSON();
    dataFrom.erase(dataFrom.find('}'), 1);
    data.replace(data.find('{'),1,",");
    std::string result = dataFrom + data;
    return result;
}

Error::Error(const std::string &clientUUID, const std::string &errorMessage = ""):
ServerCommand(ERROR, clientUUID, fail, errorMessage) {}
