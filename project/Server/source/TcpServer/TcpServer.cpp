//
// Created by paul_s on 16.04.2020.
//

#include "TcpServer.hpp"

TcpServer::TcpServer(const std::string &ipAddress, unsigned int port, unsigned int timeout /* queueManager */) :
    _acceptor(_service) {
  ipAddress.size();
  size_t i = port + timeout;
  i++;
}


TcpServer::~TcpServer() {

}


void TcpServer::startServer() {

}


void TcpServer::stopServer() {

}


void TcpServer::startAccept() {

}


void TcpServer::handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& ec) {
  client->read();
  ec.value();
}


void TcpServer::runService() {

}


void TcpServer::runQueueWorker() {

}