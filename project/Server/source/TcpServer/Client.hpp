//
// Created by paul_s on 16.04.2020.
//

#pragma once
#include <boost/asio.hpp>
#include <queue>
#include <mutex>

class TcpServer;

class Client {
public:
  // all of them must catch mutex?
  Client(boost::asio::io_service &io, TcpServer &tcpServer);
  void read();
  void handleRead(const boost::system::error_code& ec, size_t n_bytes);
  void write();
  void handleWrite(const boost::system::error_code& ec, size_t n_bytes);
  void putDataToSend(const std::string &data);


private:
  void putRecvDataToQueue(const std::string &data); // except this?

  boost::asio::ip::tcp::socket _sock;
  bool _isWriting;
  std::string _connectionUUID;
  char _recvBuf[1024];
  char _sendBuf[1024];
  std::string _recv;
  std::queue<std::string> _dataToSendQueue;
  TcpServer &_tcpServer;

  std::mutex _clientMutex;

};

