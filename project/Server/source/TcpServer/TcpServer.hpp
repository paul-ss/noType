//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "Client.hpp"
#include "QueueManager.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <unordered_map>
#include <memory>
#include <thread>
#include <string>
#include <iostream>


enum ServerState {SERVER_RUN, SERVER_STOP};


class TcpServer {
public:

  TcpServer(std::shared_ptr<QueueManager> queueManager, const std::string &ipAddress, unsigned int port,
            unsigned int timeout);
  ~TcpServer(); // join
  void startServer();
  void stopServer();
  void pushToQueue(const std::string &data, const std::string &connectionUUID);
  void removeConnection(const std::string &connectionUUID);

private:
  void startAccept();
  void handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& ec);

  void runService();
  void runQueueWorker();

  std::string randomUUID();


  boost::asio::io_service _service;
  boost::asio::ip::tcp::acceptor _acceptor;
  std::unordered_map<std::string, std::shared_ptr<Client>> _clients;
  std::vector<std::thread> _threads;
  std::shared_ptr<QueueManager> _queueManager;
  boost::asio::ip::tcp::endpoint _ep;
  ServerState _state;
  std::mutex _serverMutex;
};


