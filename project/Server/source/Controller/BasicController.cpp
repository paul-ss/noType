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
        //std::cout << "BC : invalid command type" << std::endl;
        BOOST_LOG_TRIVIAL(error) << "BC : invalid command type";

        auto commandResp = std::make_shared<ErrorResponse>(
            command->getConnectionUUID(),
            "BC : invalid command type");
        _queueManager->controllerPush(commandResp);

    }

  } catch (...) {
    //std::cout << "BC : exception : unknown, unexpected, mysterious... (like James Bond)" << std::endl;
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
    //std::cout <<"Can't cast command " + std::string(typeid(command).name()) << std::endl;
    //std::cout <<"handler type: " + std::string(typeid(handler).name()) << std::endl;
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
    // todo log

  } catch (...) {
    auto commandResp = std::make_shared<ResponseCommand>(castedCmd->getConnectionUUID());
    commandResp->setError("Exception : unknown exception type");
    _queueManager->controllerPush(commandResp);
    // todo log
  }
}



void BasicController::initHandler(const std::shared_ptr<InitRequest> &command) {
  auto commandResp = std::make_shared<InitResponse>(
      command->getConnectionUUID(),
      randomUUID());
  // todo work with db

  _queueManager->controllerPush(commandResp);
}





void BasicController::connectHandler(const std::shared_ptr<ConnectRequest> &command) {
  auto commandResp = std::make_shared<ConnectResponse>(
      command->getConnectionUUID());
  // todo work with db

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

  //std::cout << "QueueWorker (BC) stopped" << std::endl;
  BOOST_LOG_TRIVIAL(info) << "QueueWorker (BC) stopped";
}



std::string BasicController::randomUUID() {
  boost::uuids::random_generator gen;
  boost::uuids::uuid uuid = gen();
  return to_string(uuid);
}