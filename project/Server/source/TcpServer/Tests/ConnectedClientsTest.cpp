//
// Created by paul_s on 05.05.2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Client.hpp"
#include "ConnectedClients.hpp"



class ConnectedClientsTest : public ::testing::Test {
protected:
  void SetUp() override {
    delim = "delim";
    clients = std::make_shared<ConnectedClients>();
    qm = std::make_shared<QueueManager>();
    client = std::make_shared<Client>(service, clients, qm, "uuid", delim);
  }

  void TearDown() override {}

  std::string delim;
  boost::asio::io_service service;
  std::shared_ptr<ConnectedClients> clients;
  std::shared_ptr<QueueManager> qm;
  std::shared_ptr<Client> client;
};



TEST_F(ConnectedClientsTest, insert) {
  ASSERT_TRUE(clients->insert(client));
  ASSERT_EQ(clients->getClient("uuid"), client);
  ASSERT_FALSE(clients->insert(client));
}


TEST_F(ConnectedClientsTest, erase) {
  ASSERT_FALSE(clients->erase("uuid"));
  ASSERT_TRUE(clients->insert(client));
  ASSERT_TRUE(clients->erase("uuid"));
  ASSERT_FALSE(clients->erase("uuid"));
}


TEST_F(ConnectedClientsTest, get_client) {
  ASSERT_THROW(clients->getClient("uuid"), std::out_of_range);
  ASSERT_TRUE(clients->insert(client));
  ASSERT_NO_THROW(clients->getClient("uuid"));
  ASSERT_EQ(clients->getClient("uuid"), client);
}