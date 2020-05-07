//
// Created by paul_s on 16.04.2020.
//

#include "Client.hpp"
#include "TcpServer.hpp"


Client::Client(boost::asio::io_service &io,
                const std::weak_ptr<ConnectedClients> &clients,
                const std::shared_ptr<QueueManager> &queueManager,
                const std::string &connectionUUID,
                const std::string &delim) :
    _sock(io),
    _isWriting(false),
    _connectionUUID(connectionUUID),
    _clients(clients),
    _queueManager(queueManager),
    _delim(delim) {}


void Client::read() {
  std::unique_lock<std::mutex> lock(_clientMutex);
  boost::asio::async_read_until(_sock, _recvBuf, _delim, boost::bind(&Client::handleRead, shared_from_this(), _1, _2));
}


void Client::handleRead(const boost::system::error_code& ec, size_t n_bytes) {
  {
    std::unique_lock<std::mutex> lock(_clientMutex);
    // deadlock??
    if (ec) {
      if (ec == boost::asio::error::eof) {
        std::cout << "disconnected" << std::endl; //todo log
      }

      removeThisConnection();

      std::cout << ec.message() << std::endl; // todo log
      return;
    }


    std::istream is(&_recvBuf);
    std::string data(n_bytes, '0');

    if (is.readsome(&data[0], n_bytes) == (long) n_bytes) {
      data.erase(data.begin() + data.rfind(_delim), data.end());
      std::cout << "server push" << std::endl;
      _queueManager->serverPush(data, _connectionUUID);
    } else {
      //todo log
    }

  }

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

  if(_dataToSendQueue.front().find(_delim) != std::string::npos) {
    std::cout << "delim found" << std::endl;
  }

  _sendBuf = _dataToSendQueue.front() + _delim;
  _dataToSendQueue.pop();
  _isWriting = true;


  boost::asio::async_write(
      _sock,
      boost::asio::buffer(_sendBuf, _sendBuf.size()),
      boost::bind(&Client::handleWrite, shared_from_this(), _1, _2));
}


void Client::handleWrite(const boost::system::error_code& ec, size_t n_bytes) {
  {
    std::unique_lock<std::mutex> lock(_clientMutex);
    if (ec) {
      removeThisConnection();
      std::cout << ec.message() << std::endl; // todo log
      return;
    }


    if (n_bytes != _sendBuf.size()) {
      std::cout << "handleWrite error" << std::endl; // todo log
    }

    _isWriting = false;
    _sendBuf.clear();
  }

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
  std::unique_lock<std::mutex> lock(_clientMutex);
  return _sock;
}


std::string Client::getConnectionUUID() {
  std::unique_lock<std::mutex> lock(_clientMutex);
  return _connectionUUID;
}


void Client::putRecvDataToQueue(const std::string &data) {
  data.size();
}


void Client::removeThisConnection() {
  auto clientsShared = _clients.lock();
  if (clientsShared) {
    if (!clientsShared->erase(_connectionUUID)) {
      std::cout << "RemoveThisConnection error: can't erase client" << std::endl;
    }

  } else {
    std::cout << "RemoveThisConnection error: can't make shared" << std::endl; //todo log
  }
}