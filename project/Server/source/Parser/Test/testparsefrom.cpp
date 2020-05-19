//
// Created by foxyuiopkha on 04.05.2020.
//
#include <CommandFactory.hpp>
#include "gtest/gtest.h"
#include <sstream>
#include <fstream>
#define file_init "../source/Parser/Test/test_json/test_init.json"
#define file_connect "../source/Parser/Test/test_json/test_connect.json"
#define file_start_game "../source/Parser/Test/test_json/test_startgame.json"
#define file_get_text "../source/Parser/Test/test_json/test_textreq.json"
#define file_room_status "../source/Parser/Test/test_json/test_statusreq.json"
#define file_validate "../source/Parser/Test/test_json/test_writtentextreq.json"

int main() {
    std::ifstream f(file_init);
    std::stringstream ss_in;
    ss_in << f.rdbuf();
    f.close();
    std::string json_init = ss_in.str();
    std::cout << json_init << std::endl;
    CommandFactory CF;
    std::shared_ptr<ClientCommand> command;
    command = CF.createCommand(json_init);
    std::cout << command->getConnectionUUID() << std::endl;
    std::cout << command->getTypeOfController() << std::endl;
    std::cout << command->getTypeOfCommand() << std::endl;
    std::cout << std::dynamic_pointer_cast<InitRequest>(command)->name << std::endl;
    std::cout << std::endl;

    f.open(file_connect);
    std::stringstream ss;
    ss << f.rdbuf();

    std::string json_connect = ss.str();
    std::cout << json_connect << std::endl;
    command = CF.createCommand(json_connect);
    std::cout << command->getConnectionUUID() << std::endl;
    std::cout << command->getTypeOfController() << std::endl;
    std::cout << command->getTypeOfCommand() << std::endl;
    std::cout << std::dynamic_pointer_cast<ConnectRequest>(command)->key << std::endl;
    std::cout << std::endl;

    return 0;
}
