//
// Created by paul_s on 16.04.2020.
//

#include "Client.hpp"


Client::Client(boost::asio::io_service &io, TcpServer &tcpServer) :
    _sock(io),
    _isWriting(false),
    _tcpServer(tcpServer) {

}


void Client::read() {

}


void Client::handleRead(const boost::system::error_code& ec, size_t n_bytes) {
  ec.value();
  n_bytes++;
}


void Client::write() {

}


void Client::handleWrite(const boost::system::error_code& ec, size_t n_bytes) {
  ec.value();
  n_bytes++;
}


void Client::putDataToSend(const std::string &data) {
  data.size();
}


void Client::putRecvDataToQueue(const std::string &data) {
  data.size();
}