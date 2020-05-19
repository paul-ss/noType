//
// Created by paul_s on 13.05.2020.
//

#include "gtest/gtest.h"


#include "Init.hpp"
#include "Connect.hpp"
#include "GetRoomStatus.hpp"
#include "GetText.hpp"
#include "StartGameSession.hpp"
#include "ValidateWrittenText.hpp"

#include <thread>






class ParseFromTest : public ::testing::Test {
public:
  std::string getJSON(CommandType command) {
    return "{\n \"" COMMAND_TYPE_JSON_PATH "\" : \"" + std::string(command._to_string()) + "\",\n" +
           "\"" CLIENT_UUID_JSON_PATH "\" : \"" + clientUUID + "\",\n" +
           "\"" CLIENT_NAME_JSON_PATH "\" : \"" + name + "\",\n" +
           "\"" WRITTEN_TEXT_JSON_PATH "\" : \"" + text + "\"\n }";
  }


protected:


  void SetUp() override {
    commandType = "command_typesadfdsf";
    clientUUID = "clientUUIDdfgaf";
    connectionUUID = "commectionUUIDjobobo";
    name = "nameasdfasdf";
    text = "textasdfasdfasd";
  }

  void TearDown() override {}
  std::string commandType;
  std::string clientUUID;
  std::string connectionUUID;
  std::string name;
  std::string text;
  std::string json;
};


TEST_F(ParseFromTest, InitRequest) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::InitRequest));
  pt::read_json(stream, tree);


  InitRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::InitRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::basic);
  ASSERT_EQ(cmd.getName(), name);
}


TEST_F(ParseFromTest, ConnectRequest) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::ConnectRequest));
  pt::read_json(stream, tree);


  ConnectRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::ConnectRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::basic);
  ASSERT_EQ(cmd.getClientUUID(), clientUUID);
}


TEST_F(ParseFromTest, StartGame) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::StartGameSessionRequest));
  pt::read_json(stream, tree);


  StartGameSessionRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::StartGameSessionRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::game);
  ASSERT_EQ(cmd.getClientUUID(), clientUUID);
}


TEST_F(ParseFromTest, GetText) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::GetTextRequest));
  pt::read_json(stream, tree);


  GetTextRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::GetTextRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::game);
  ASSERT_EQ(cmd.getClientUUID(), clientUUID);
}



TEST_F(ParseFromTest, GetStatus) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::RoomStatusRequest));
  pt::read_json(stream, tree);


  RoomStatusRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::RoomStatusRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::game);
  ASSERT_EQ(cmd.getClientUUID(), clientUUID);
}



TEST_F(ParseFromTest, validateText) {
  pt::ptree tree;
  std::stringstream stream(getJSON(CommandType::ValidateWrittenTextRequest));
  pt::read_json(stream, tree);


  ValidateWrittenTextRequest cmd(connectionUUID, std::move(tree));
  ASSERT_EQ(cmd.getCommandType()._to_string(), (+CommandType::ValidateWrittenTextRequest)._to_string());
  ASSERT_EQ(cmd.getConnectionUUID(), connectionUUID);
  ASSERT_TRUE(cmd.getControllerType() == ControllerType::game);
  ASSERT_EQ(cmd.getClientUUID(), clientUUID);
  ASSERT_EQ(cmd.getWrittenText(), text);
}