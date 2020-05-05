//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "ConnectedClients.hpp"
#include "QueueManager.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <queue>
#include <mutex>
#include <iostream>



class Client : public std::enable_shared_from_this<Client> {
public:
  // all of them must catch mutex?
  Client(boost::asio::io_service &io,
         const std::weak_ptr<ConnectedClients> &clients,
         const std::shared_ptr<QueueManager> &queueManager,
         const std::string &connectionUUID,
         const std::string &delim);
  void read();
  void handleRead(const boost::system::error_code& ec, size_t n_bytes);
  void write();
  void handleWrite(const boost::system::error_code& ec, size_t n_bytes);
  void putDataToSend(const std::string &data);
  boost::asio::ip::tcp::socket &sock();
  std::string getConnectionUUID();


private:
  void putRecvDataToQueue(const std::string &data); // except this?
  void removeThisConnection();

  boost::asio::ip::tcp::socket _sock;
  bool _isWriting;
  std::string _connectionUUID;
  boost::asio::streambuf _recvBuf;
  std::string _sendBuf;
  std::queue<std::string> _dataToSendQueue;
  std::weak_ptr<ConnectedClients> _clients;
  std::shared_ptr<QueueManager> _queueManager;
  std::string _delim;
  std::mutex _clientMutex;

};

