//
// Created by paul_s on 04.05.2020.
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Client.hpp"
#include "ConnectedClients.hpp"


#include <thread>

class MockClients : public ConnectedClients {
public:
  MOCK_METHOD1(erase, bool(const std::string &));
};


class FakeQueueManager : public QueueManager {
public:
  void serverPush(const std::string &data, const std::string &connectionUUID) override {
    recv.emplace_back(data, connectionUUID);
  }

  struct Recv {
    Recv(const std::string &data, const std::string &uuid) : data(data), uuid(uuid) {}
    std::string data;
    std::string uuid;
  };

  std::vector<Recv> recv;
};



class ClientTest : public ::testing::Test {
protected:
  void SetUp() override {
    delim = "delim";
    clients = std::make_shared<MockClients>();
    qm = std::make_shared<FakeQueueManager>();
    client = std::make_shared<Client>(service, clients, qm, "uuid", delim);

    ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10000);
  }

  void TearDown() override {}

  std::string delim;
  boost::asio::io_service service;
  std::shared_ptr<MockClients> clients;
  std::shared_ptr<FakeQueueManager> qm;
  std::shared_ptr<Client> client;
  boost::asio::ip::tcp::endpoint ep;
};



std::string makeString(size_t length) {
  std::string result;
  for (size_t i = 0; i < length; ++i) {
    char sym = rand();
    result += sym;
  }

  return result;
}


void readData(std::string &res, boost::asio::streambuf &buf, boost::asio::ip::tcp::socket &sock, const std::string &delim) {
  size_t bytes = read_until(sock, buf, delim);
  if (bytes == 0) {
    FAIL();
  }

  std::istream is(&buf);
  std::string data(bytes, '0');

  if (is.readsome(&data[0], bytes) != (long) bytes) {
    FAIL();
  }

  res = data.substr(0, data.size() - std::string(delim).size());
}



TEST_F(ClientTest, read) {
  std::vector<std::thread> thr;

  boost::asio::ip::tcp::acceptor acceptor(service, ep);
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.listen(1024);

  thr.emplace_back([&]() {
    acceptor.accept(client->sock());
    client->read();
  });

  boost::asio::io_service::work w(service);
  thr.emplace_back([&]() {service.run();});

  boost::asio::ip::tcp::socket sock(service);
  sock.open(boost::asio::ip::tcp::v4());
  sock.connect(ep);

  std::string msg1(makeString(11) + delim);
  write(sock, boost::asio::buffer(msg1, msg1.size()));
  std::string msg2(makeString(1123123) + delim);
  write(sock, boost::asio::buffer(msg2, msg2.size()));
  std::string msg3(makeString(223123) + delim);
  write(sock, boost::asio::buffer(msg3, msg3.size()));

  int i = 0;
  while (qm->recv.size() != 3) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    i++;
    if (i == 10) {
      FAIL();
    }
  }

  sock.close();
  service.stop();


  for (auto &t : thr) {
    t.join();
  }


  ASSERT_EQ(qm->recv.size(), 3);
  ASSERT_EQ(qm->recv[0].data + delim, msg1);
  ASSERT_EQ(qm->recv[1].data + delim, msg2);
  ASSERT_EQ(qm->recv[0].uuid, "uuid");
  ASSERT_EQ(qm->recv[1].uuid, "uuid");
}



TEST_F(ClientTest, write) {
  std::vector<std::thread> thr;

  boost::asio::ip::tcp::acceptor acceptor(service, ep);
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.listen(1024);

  std::vector<std::string> msgs;
  thr.emplace_back([&]() {
    acceptor.accept(client->sock());

    msgs.emplace_back(makeString(1309438));
    msgs.emplace_back(makeString(1023034));
    msgs.emplace_back(makeString(125214));
    client->putDataToSend(msgs[0]);
    client->putDataToSend(msgs[1]);
    client->putDataToSend(msgs[2]);
  });

  boost::asio::io_service::work w(service);
  thr.emplace_back([&]() {service.run();});

  boost::asio::ip::tcp::socket sock(service);
  sock.open(boost::asio::ip::tcp::v4());
  sock.connect(ep);

  boost::asio::streambuf buf;
  for (int i = 0; i < 3; i++) {
    std::string res;
    readData(res, buf, sock, delim);
    EXPECT_EQ(msgs[i].size(), res.size());
    EXPECT_EQ(msgs[i], res);
  }

  service.stop();

  for (auto &t : thr) {
    t.join();
  }

}

TEST(Client, run_call) {
  auto mc = std::make_shared<MockClients>();
  EXPECT_CALL(*mc.get(), erase("uuid")).Times(1);

  boost::asio::io_service service;
  auto qm = std::make_shared<FakeQueueManager>();
  auto client = std::make_shared<Client>(service, mc, qm, "uuid", "delim");
  client->read();

  std::thread t([&]() { service.run(); });

  t.join();
}