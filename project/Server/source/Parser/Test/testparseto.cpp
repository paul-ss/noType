//
// Created by foxyuiopkha on 10.05.2020.
//
#include <Command.hpp>
#include "gtest/gtest.h"
#include <sstream>

int main(){
    std::string clientUUID = "test_client";
    std::string key = "test_key";
    std::string playerID = "test_player";
    double waitTime = 0.012;
    std::string text = "test_text";
    std::string error = "error";
    status state(fail);
    std::string result;

    InitResponse init(clientUUID, key);
    result = init.parseToJSON();
    std::cout << result << std::endl;

    InitResponse initError(clientUUID, state, error);
    result = initError.parseToJSON();
    std::cout << result << std::endl;

    ConnectResponse connect(clientUUID);
    result = connect.parseToJSON();
    std::cout << result << std::endl;

    ConnectResponse connectError(clientUUID, state, error);
    result = connectError.parseToJSON();
    std::cout << result << std::endl;


    StartGameSessionResponse start(clientUUID, playerID, waitTime);
    result = start.parseToJSON();
    std::cout << result << std::endl;

    StartGameSessionResponse startError(clientUUID, state, error);
    result = startError.parseToJSON();
    std::cout << result << std::endl;

    GetTextResponse gettext(clientUUID, text);
    result = gettext.parseToJSON();
    std::cout << result << std::endl;

    Error err(clientUUID, error);
    result = err.parseToJSON();
    std::cout << result << std::endl;

}