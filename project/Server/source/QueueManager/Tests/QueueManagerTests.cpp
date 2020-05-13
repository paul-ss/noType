//
// Created by paul_s on 06.05.2020.
//


#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Queue.hpp"
#include "QueueManager.hpp"

using ::testing::Return;

class MockQueue : public Queue {
public:
  MOCK_METHOD1(push, void(const std::shared_ptr<Command> &));
  MOCK_METHOD1(pop, bool(std::shared_ptr<Command> &));
  MOCK_METHOD0(notify, void());
};


class FakeClientCommand : public ClientCommand {
  void parseFromPtree(pt::ptree &&pTree) override {
    pTree.empty();
  }
};

class FakeCommandFactory : public CommandFactory {
  std::shared_ptr<ClientCommand> createCommand(const std::string &connectionUUID,
                                                  const std::string &data) override {
    data.size();
    connectionUUID.size();
    return std::make_shared<FakeClientCommand>();
  }
};


class QueueManagerTest : public ::testing::Test {
protected:
  void SetUp() override {
    qServer = std::make_shared<MockQueue>();
    qGameCtr = std::make_shared<MockQueue>();
    qBasicCtr = std::make_shared<MockQueue>();
    commFactory = std::make_shared<FakeCommandFactory>();
  }

  void TearDown() override {}

  std::shared_ptr<MockQueue> qServer;
  std::shared_ptr<MockQueue> qGameCtr;
  std::shared_ptr<MockQueue> qBasicCtr;
  std::shared_ptr<CommandFactory> commFactory;
  std::shared_ptr<Command> command;
};


TEST_F (QueueManagerTest, server_pop) {
  QueueManager qm(qServer, qBasicCtr, qGameCtr, commFactory);
  EXPECT_CALL(*qServer, pop(command)).WillOnce(Return(true));
  qm.serverPop(command);
}

TEST_F (QueueManagerTest, server_push) {
 // todo implement it
}

TEST_F (QueueManagerTest, controller_push) {
  QueueManager qm(qServer, qBasicCtr, qGameCtr, commFactory);
  EXPECT_CALL(*qServer, push(command)).WillOnce(Return());
  qm.controllerPush(command);
}

TEST_F (QueueManagerTest, game_pop) {
  QueueManager qm(qServer, qBasicCtr, qGameCtr, commFactory);
  EXPECT_CALL(*qGameCtr, pop(command)).WillOnce(Return(true));
  qm.gameControllerPop(command);
}

TEST_F (QueueManagerTest, basic_pop) {
  QueueManager qm(qServer, qBasicCtr, qGameCtr, commFactory);
  EXPECT_CALL(*qBasicCtr, pop(command)).WillOnce(Return(true));
  qm.basicControllerPop(command);
}

TEST_F (QueueManagerTest, server_notify) {
  QueueManager qm(qServer, qBasicCtr, qGameCtr, commFactory);
  EXPECT_CALL(*qServer, notify()).WillOnce(Return());
  qm.serverNotify();
}