//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "Client.hpp"

#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <string>


class TcpServer {
public:

  TcpServer(const std::string &ipAddress, unsigned int port, unsigned int timeout /* queueManager */);
  ~TcpServer(); // join
  void startServer();
  void stopServer();

private:
  void startAccept();
  void handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& ec);

  void runService();
  void runQueueWorker();


  boost::asio::io_service _service;
  boost::asio::ip::tcp::acceptor _acceptor;
  std::vector<std::shared_ptr<Client>> _clients;
  std::vector<std::thread> _threads;
  enum {SERVER_RUN, SERVER_STOP} _state;

// std::mutex _serverMutex;
//  - _queueManager : std::shared_ptr<QueueManager>

};


