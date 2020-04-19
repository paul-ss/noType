//
// Created by paul_s on 16.04.2020.
//

#include "TcpServer.hpp"

TcpServer::TcpServer(std::shared_ptr<QueueManager> queueManager, const std::string &ipAddress, unsigned int port,
                     unsigned int timeout) :
    _acceptor(_service),
    _queueManager(queueManager),
    _ep(boost::asio::ip::address::from_string(ipAddress), port),
    _state(SERVER_STOP) {
  timeout++;  // TODO (PavelS) Implement it
  // ip::tcp::v4()
 // _queueManager = queueManager;
}


TcpServer::~TcpServer() {
  for (auto &thread : _threads) {
    thread.join();
  }
}


void TcpServer::startServer() {
  {
    std::unique_lock<std::mutex> lock(_serverMutex);

    _acceptor.open(_ep.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(_ep);
    _acceptor.listen(1024);  // To config
  }

  startAccept();

  {
    std::unique_lock<std::mutex> lock(_serverMutex);
    // worker threads
    for (size_t i = 0; i < 2; i++) {  // To config
      _threads.push_back(std::thread(std::bind(&TcpServer::runService, this)));
    }

    _threads.push_back(std::thread(std::bind(&TcpServer::runQueueWorker, this)));
    _state = SERVER_RUN;
  }
}


void TcpServer::stopServer() {
  std::unique_lock<std::mutex> lock(_serverMutex);
  _service.stop();
  _state = SERVER_STOP;
}


void TcpServer::startAccept() {
  std::unique_lock<std::mutex> lock(_serverMutex);
  std::shared_ptr<Client> c(new Client(_service, *this, randomUUID()));
  _acceptor.async_accept(c->sock(), boost::bind(&TcpServer::handleAccept, this, c, boost::asio::placeholders::error));
}


void TcpServer::handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& ec) {
  {
    std::unique_lock<std::mutex> lock(_serverMutex);
    if (!ec) {
      auto pair = _clients.insert(std::pair<std::string, std::shared_ptr<Client>>(client->getConnectionUUID(), client));
      if (!pair.second) {
        std::cerr << "Map error, client was not added" << std::endl; // todo log
      }
      client->read();

    } else {
      std::cerr << ec.value() << " " << ec.message() << std::endl; // todo log
    }
  }

  startAccept();
}


void TcpServer::runService() {
  _service.run();
}

void TcpServer::pushToQueue(const std::string &data, const std::string &connectionUUID) {
  _queueManager->serverPush(data, connectionUUID);
  // errors?
}


void TcpServer::removeConnection(const std::string &connectionUUID) {
  std::unique_lock<std::mutex> lock(_serverMutex);
  if (_clients.count(connectionUUID) > 0) {
    _clients.erase(connectionUUID);
  } else {
    // todo log
  }
}


void TcpServer::runQueueWorker() {
  while(1) {
    {
      std::unique_lock<std::mutex> lock(_serverMutex);
      if (_state == SERVER_STOP) {
        break;
      }
    }

    std::shared_ptr<Command> command;
    if (_queueManager->serverPop(command)) {
      //std::cout << "ServerPop " << command->data << "\n" << command->connectionUUID << std::endl;
      std::unique_lock<std::mutex> lock(_serverMutex);
      if (_clients.count(command->connectionUUID) > 0) {
       // std::cout << "ServerPop putData " << command->data << std::endl;
        _clients[command->connectionUUID]->putDataToSend(command->parseToJSON());
      } else {
        // todo log
      }
    }
  }
}

std::string TcpServer::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}
