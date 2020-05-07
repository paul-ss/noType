#include "logger.hpp"
#include "game.hpp"
#include <iostream>
#include <filesystem>
#include "utils.hpp"

int main() {
    init_logger();
    //BOOST_LOG_TRIVIAL(info) << "notope start";
    Game game;
    game.Run();
    return 0;
}
