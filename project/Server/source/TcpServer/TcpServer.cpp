//
// Created by paul_s on 16.04.2020.
//

#include "TcpServer.hpp"

TcpServer::TcpServer(const std::shared_ptr<QueueManager> &queueManager, const ServerConfig &config) :
    _acceptor(_service),
    _clients (new ConnectedClients()),
    _queueManager(queueManager),
    _ep(boost::asio::ip::address::from_string(config._ip), config._port),
    _state(SERVER_STOP),
    _config(config) {}


TcpServer::~TcpServer() {
  for (auto &thread : _threads) {
    thread.join();
  }
}


void TcpServer::startServer() {
  _acceptor.open(_ep.protocol());
  _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  _acceptor.bind(_ep);
  _acceptor.listen(_config._listenCount);


  startAccept();

  {
    std::unique_lock<std::mutex> lock(_serverMutex);
    // worker threads
    for (size_t i = 0; i < _config._workerThreadsCount; i++) {
      _threads.emplace_back(std::bind(&TcpServer::runService, this));
    }

    for (size_t i = 0; i < _config._queueWorkersCount; i++) {
      _threads.emplace_back(std::bind(&TcpServer::runQueueWorker, this));
    }
    _state = SERVER_RUN;
  }
}


void TcpServer::stopServer() {
  std::unique_lock<std::mutex> lock(_serverMutex);
  _service.stop();
  _state = SERVER_STOP;
  _queueManager->serverNotify();
}


void TcpServer::startAccept() {
  std::unique_lock<std::mutex> lock(_serverMutex);
  std::shared_ptr<Client> c(new Client(_service,
                                              _clients,
                                              _queueManager,
                                              randomUUID(),
                                              _config._delim));
  _acceptor.async_accept(c->sock(), boost::bind(&TcpServer::handleAccept, this, c, boost::asio::placeholders::error));
}


void TcpServer::handleAccept(std::shared_ptr<Client> client, const boost::system::error_code& ec) {
  if (!ec) {
    if (!_clients->insert(client)) {
      std::cout << "Map error, client was not added" << std::endl; // todo log
    }
    client->read();

  } else {
    std::cout << ec.value() << " " << ec.message() << std::endl; // todo log
  }


  startAccept();
}


void TcpServer::runService() {
  _service.run();
}

//void TcpServer::pushToQueue(const std::string &data, const std::string &connectionUUID) {
//  _queueManager->serverPush(data, connectionUUID);
//  // errors?
//}



void TcpServer::runQueueWorker() {
  while(true) {
    {
      std::unique_lock<std::mutex> lock(_serverMutex);
      if (_state == SERVER_STOP) {
        break;
      }
    }

    std::shared_ptr<Command> command;
    if (_queueManager->serverPop(command)) {
      try {
        auto client = _clients->getClient(command->connectionUUID);
        client->putDataToSend(command->parseToJSON());

      } catch(const std::out_of_range& exc) {
        // todo log
        std::cout << exc.what() << std::endl;
      }
    }
  }

  std::cout << "Queue Worker stopped" << std::endl; // todo log
}

std::string TcpServer::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}
