//
// Created by paul_s on 16.04.2020.
//

#include "BasicController.hpp"
#include "Logger.hpp"



BasicController::BasicController(const std::shared_ptr<QueueManager> &queueManager,
                const std::shared_ptr<IDataBaseFacade> &dataBaseFacade) :
    _queueManager(queueManager),
    _dataBaseFacade(dataBaseFacade),
    _work(_service),
    _state(BASIC_CONTROLLER_STOP) {}



BasicController::~BasicController() {
  for (auto &thread : _threads) {
    thread.join();
  }
}



void BasicController::startController() {
  std::unique_lock<std::mutex> lock(_basicControllerMutex);


  for (size_t i = 0; i < 1; i++) {
    _threads.emplace_back(std::bind(&BasicController::runQueueWorker, this));
  }

  _state = BASIC_CONTROLLER_RUN;
}



void BasicController::stopController() {
  std::unique_lock<std::mutex> lock(_basicControllerMutex);
  _service.stop();
  _state = BASIC_CONTROLLER_STOP;
  _queueManager->basicControllerNotify();
}







void BasicController::commandDistributor(const std::shared_ptr<Command> &command) {
  try {
    auto type = command->getCommandType();

    switch (type) {
      case (CommandType::InitRequest) :
        handlerExceptionCatcher<InitRequest, InitResponse>(
            command,
            boost::bind(&BasicController::initHandler, this, _1));
        break;

      case (CommandType::ConnectRequest) :
        handlerExceptionCatcher<ConnectRequest, ConnectResponse>(
            command,
            boost::bind(&BasicController::connectHandler, this, _1));
        break;

      case (CommandType::ErrorRequest) :
        handlerExceptionCatcher<ErrorRequest, ErrorResponse>(
            command,
            boost::bind(&BasicController::errorHandler, this, _1));
        break;

      default:
        BOOST_LOG_TRIVIAL(error) << "BC : invalid command type";

        auto commandResp = std::make_shared<ErrorResponse>(
            command->getConnectionUUID(),
            "BC : invalid command type");
        _queueManager->controllerPush(commandResp);

    }

  } catch (...) {
    BOOST_LOG_TRIVIAL(error) << "BC : exception : unknown, unexpected, mysterious... (like James Bond)";

    auto commandResp = std::make_shared<ErrorResponse>(
        command->getConnectionUUID(),
        "BC : exception : unknown, unexpected, mysterious... (like James Bond)" );
    _queueManager->controllerPush(commandResp);
  }
}




template <class RequestCommand, class ResponseCommand, typename CommandHandler>
void BasicController::handlerExceptionCatcher(const std::shared_ptr<Command> &command, CommandHandler handler) {
  auto castedCmd = std::dynamic_pointer_cast<RequestCommand>(command);
  if (!castedCmd) {
    BOOST_LOG_TRIVIAL(error) << "Can't cast command " + std::string(typeid(command).name()) << std::endl
    << "handler type: " + std::string(typeid(handler).name());


    auto commandResp = std::make_shared<ErrorResponse>(
        castedCmd->getConnectionUUID(),
        "Can't cast command " + std::string(typeid(command).name()));
    _queueManager->controllerPush(commandResp);

    return;
  }

  try {
    handler(castedCmd);

  } catch (std::exception &exc) {
    auto commandResp = std::make_shared<ResponseCommand>(castedCmd->getConnectionUUID());
    commandResp->setError(exc.what());
    _queueManager->controllerPush(commandResp);

    BOOST_LOG_TRIVIAL(error) << "BC : exception : " << exc.what();

  } catch (...) {
    auto commandResp = std::make_shared<ResponseCommand>(castedCmd->getConnectionUUID());
    commandResp->setError("Exception : unknown exception type");
    _queueManager->controllerPush(commandResp);

    BOOST_LOG_TRIVIAL(error) << "BC : exception : unknown exception type";
  }
}



void BasicController::initHandler(const std::shared_ptr<InitRequest> &command) {
  auto clientUUID = randomUUID();

  _dataBaseFacade->InsertPlayerInfo(std::make_unique<DataBase::External::PlayerInfo>(clientUUID));

  auto commandResp = std::make_shared<InitResponse>(
      command->getConnectionUUID(),
      clientUUID);
  _queueManager->controllerPush(commandResp);
}





void BasicController::connectHandler(const std::shared_ptr<ConnectRequest> &command) {
  if (_dataBaseFacade->FindPlayerInfoByUuid(command->getClientUUID()) == nullptr) {
    auto commandResp = std::make_shared<ConnectResponse>(command->getConnectionUUID());

    commandResp->setError("connectHandler : can't find player with uuid '"
    + command->getClientUUID() + "'");

    BOOST_LOG_TRIVIAL(error) << "connectHandler : can't find player with uuid "
                             << "'" + command->getClientUUID() + "'";

    _queueManager->controllerPush(commandResp);
    return;
  }

  auto commandResp = std::make_shared<ConnectResponse>(command->getConnectionUUID());
  _queueManager->controllerPush(commandResp);
}



void BasicController::errorHandler(const std::shared_ptr<ErrorRequest> &command) {
  auto commandResp = std::make_shared<ErrorResponse>(
      command->getConnectionUUID(),
      command->getErrorMsg());

  _queueManager->controllerPush(commandResp);
}






void BasicController::runQueueWorker() {
  while(true) {
    {
      std::unique_lock<std::mutex> lock(_basicControllerMutex);
      if (_state == BASIC_CONTROLLER_STOP) {
        break;
      }
    }

    std::shared_ptr<Command> command;
    if (_queueManager->basicControllerPop(command)) {
      commandDistributor(command);
    }
  }

  BOOST_LOG_TRIVIAL(info) << "QueueWorker (BC) stopped";
}



std::string BasicController::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}