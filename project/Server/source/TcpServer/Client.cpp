//
// Created by paul_s on 16.04.2020.
//

#include "Client.hpp"
#include "TcpServer.hpp"

#define DELIM "jopa"


Client::Client(boost::asio::io_service &io, TcpServer &tcpServer, const std::string &connectionUUID) :
    _sock(io),
    _isWriting(false),
    _connectionUUID(connectionUUID),
    _tcpServer(tcpServer) {
}


void Client::read() {
  std::unique_lock<std::mutex> lock(_clientMutex);
  boost::asio::async_read_until(_sock, _recvBuf, DELIM, boost::bind(&Client::handleRead, shared_from_this(), _1, _2));
}


void Client::handleRead(const boost::system::error_code& ec, size_t n_bytes) {
  std::unique_lock<std::mutex> lock(_clientMutex);
  if (ec) {
    if (ec == boost::asio::error::eof) {
      std::cout << "disconnected" << std::endl; //todo log
    }
    _tcpServer.removeConnection(_connectionUUID);
    std::cout << ec.message() << std::endl; // todo log
    return;
  }


  std::istream is(&_recvBuf);
  std::string data(n_bytes, '0');

  if (is.readsome(&data[0], n_bytes) == (long) n_bytes) {
    data.erase(data.begin() + data.rfind(DELIM), data.end());
    std::cout << data << std::endl;
    _tcpServer.pushToQueue(data, _connectionUUID);
  } else {
    //todo log
  }

  lock.unlock();
  read();
}


void Client::write() {
  std::unique_lock<std::mutex> lock(_clientMutex);
  if (_isWriting) {
    return;
  }

  if (_dataToSendQueue.empty()) {
    _isWriting = false;
    return;
  }

  _sendBuf = _dataToSendQueue.front() + DELIM;
  _dataToSendQueue.pop();
  _isWriting = true;

  boost::asio::async_write(
      _sock,
      boost::asio::buffer(_sendBuf, _sendBuf.size()),
      boost::bind(&Client::handleWrite, shared_from_this(), _1, _2));
}


void Client::handleWrite(const boost::system::error_code& ec, size_t n_bytes) {
  std::unique_lock<std::mutex> lock(_clientMutex);
  if (ec) {
    _tcpServer.removeConnection(_connectionUUID);
    std::cout << ec.message() << std::endl; // todo log
    return;
  }

  if (n_bytes != _sendBuf.size()) {
    std::cout << "handleWrite error" << std::endl; // todo log
  }

  _isWriting = false;
  lock.unlock();
  write();
}


void Client::putDataToSend(const std::string &data) {
  {
    std::unique_lock<std::mutex> lock(_clientMutex);
    _dataToSendQueue.push(data);
  }

  write();
}

boost::asio::ip::tcp::socket &Client::sock() {
  return _sock;
}

std::string Client::getConnectionUUID() {
  return _connectionUUID;
}

void Client::putRecvDataToQueue(const std::string &data) {
  data.size();
}