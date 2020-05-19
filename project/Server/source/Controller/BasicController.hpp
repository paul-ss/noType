//
// Created by paul_s on 16.04.2020.
//

#pragma once

#include "QueueManager.hpp"
#include "DataBaseFacade.hpp"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <thread>


enum BasicControllerState {BASIC_CONTROLLER_RUN, BASIC_CONTROLLER_STOP};


class BasicController {
public:

  BasicController(const std::shared_ptr<QueueManager> &queueManager,
                 const std::shared_ptr<IDataBaseFacade> &dataBaseFacade);
  ~BasicController();
  void startController();
  void stopController();


private:
  void commandDistributor(const std::shared_ptr<Command> &command);

  template <class RequestCommand, class ResponseCommand, typename CommandHandler>
  void handlerExceptionCatcher(const std::shared_ptr<Command> &command, CommandHandler handler);

  void initHandler(const std::shared_ptr<InitRequest> &command);
  void connectHandler(const std::shared_ptr<ConnectRequest> &command);
  void errorHandler(const std::shared_ptr<ErrorRequest> &command);


  // void runClientsObserver(); ??
  void runQueueWorker();
  std::string randomUUID();




private:
  std::shared_ptr<QueueManager> _queueManager ;
  std::shared_ptr<IDataBaseFacade> _dataBaseFacade;


  boost::asio::io_service _service;
  boost::asio::io_service::work _work;
  std::mutex _basicControllerMutex;
  BasicControllerState _state;
  std::vector<std::thread> _threads;


};
