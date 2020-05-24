//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#pragma once

#include "Init.hpp"
#include "Connect.hpp"
#include "GetRoomStatus.hpp"
#include "GetText.hpp"
#include "StartGameSession.hpp"
#include "ValidateWrittenText.hpp"
#include "LeaveRoom.hpp"

#include <memory>

class CommandFactory {
public:
  virtual ~CommandFactory() = default;

  virtual std::shared_ptr<ClientCommand> createCommand(
      const std::string &connectionUUID,
      const std::string &data);

private:
  std::shared_ptr<ClientCommand> createCommandInternal(
      const std::string &connectionUUID,
      const std::string &data);
};
