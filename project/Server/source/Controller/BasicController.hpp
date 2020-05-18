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
  BasicController() = default;
  ~BasicController() = default;
  void startController();
  void stopController();


private:
  void commandDistributor(const std::shared_ptr<Command> &command);
  void startGameSessionHandler(const std::shared_ptr<Command> &command);
  void getTextHandler(const std::shared_ptr<Command> &command);
  void getRoomStatusHandler(const std::shared_ptr<Command> &command);
  void sendWrittenTextHandler(const std::shared_ptr<Command> &command);

  // void runClientsObserver(); ??
  void runQueueWorker();


private:
  std::shared_ptr<QueueManager> _queueManager ;
  std::shared_ptr<IDataBaseFacade> _dataBaseFacade;


//  boost::asio::io_service _service;
//  boost::asio::io_service::work _work;
  std::mutex _gameControllerMutex;
  BasicControllerState _state;
  std::vector<std::thread> _threads;
};


