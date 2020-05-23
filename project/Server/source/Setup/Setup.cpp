//
// Created by paul_s on 07.05.2020.
//

#include "Setup.hpp"
#include "Logger.hpp"



Setup::Setup() :
    _signalSet(_service, SIGINT, SIGTERM) {}


Setup::~Setup() {
  for (auto &t : _threads) {
    t.join();
  }
}






void Setup::parseConfig(const std::string &configPath) {
  _configParser.parseConfig(configPath);
}


void Setup::start() {
  _tcpServer->startServer();
  _gameController->startController();
  _basicController->startController();

  runSignalCatcher();

  BOOST_LOG_TRIVIAL(info) << "Server started";
}


void Setup::stop() {
  _tcpServer->stopServer();
  _gameController->stopController();
   _basicController->stopController();
}




void Setup::runSignalCatcher() {
  _signalSet.async_wait([&](const boost::system::error_code& ec, int signal_number) {
    if (!ec) {
      stop();
      BOOST_LOG_TRIVIAL(debug) << "Signal number occured " << signal_number;
      return;
    }

    BOOST_LOG_TRIVIAL(error) << "SignalCatcher : error : " << ec.message();
  });

  _threads.emplace_back([&](){ _service.run(); });

}


void Setup::setup() {


  //RoomConfig roomConfig(200, 5, 10000, 60000, 3000, 3000);


  _dataBaseFacade = std::make_shared<DataBaseFacade>();
  _queueManager = std::make_shared<QueueManager>();

  _tcpServer = std::make_unique<TcpServer>(_queueManager, _configParser.extractServerConfig());
  _gameController = std::make_unique<GameController>(_queueManager, _dataBaseFacade, _configParser.extractRoomConfig());
  _basicController = std::make_unique<BasicController>(_queueManager, _dataBaseFacade);
}
