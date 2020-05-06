//
// Created by foxyuiopkha on 04.05.2020.
//
#include <CommandFactory.hpp>

int main() {
    std::string json = "{\"commandType\":0,\"connectionUUID\":\"test_connection\",\"clientUUID\":\"test_client\","
                       "\"name\":\"test_name\"}";
    std::string json1 = "{\"commandType\":1,\"connectionUUID\":\"test_connection\",\"clientUUID\":\"test_client\","
                        "\"key\":\"test_key\"}";
    std::string json2 = "{\"commandType\":2,\"connectionUUID\":\"test_connection\",\"clientUUID\":\"test_client\","
                        "\"key\":\"test_key\"}";


    CommandFactory CF;
    std::shared_ptr<ClientCommand> command;
    command = CF.createCommand(json);
    std::cout << command->getClientUUID() << std::endl;
    std::cout << command->getConnectionUUID() << std::endl;
    std::cout << command->getTypeOfController() << std::endl;
    std::cout << command->getTypeOfCommand() << std::endl;
    std::cout << std::dynamic_pointer_cast<InitRequest>(command)->name << std::endl;
    std::cout << std::endl;
    command = CF.createCommand(json1);
    std::cout << command->getClientUUID() << std::endl;
    std::cout << command->getConnectionUUID() << std::endl;
    std::cout << command->getTypeOfController() << std::endl;
    std::cout << command->getTypeOfCommand() << std::endl;
    std::cout << std::dynamic_pointer_cast<ConnectRequest>(command)->key << std::endl;
    std::cout << std::endl;
    command = CF.createCommand(json2);
    std::cout << command->getClientUUID() << std::endl;
    std::cout << command->getConnectionUUID() << std::endl;
    std::cout << command->getTypeOfController() << std::endl;
    std::cout << command->getTypeOfCommand() << std::endl;
    std::cout << std::dynamic_pointer_cast<StartGameSessionRequest>(command)->key << std::endl;
    return 0;
}
