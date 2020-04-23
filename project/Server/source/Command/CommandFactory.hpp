//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#pragma once
#include <Command.hpp>
#include <memory>

class CommandFactory {
 public:
    virtual std::shared_ptr<ClientCommand> createCommand(const std::string &data);
};


