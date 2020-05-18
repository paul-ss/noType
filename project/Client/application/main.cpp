#include "logger.hpp"
#include "game.hpp"

int main() {
    init_logger();
    BOOST_LOG_TRIVIAL(info) << "notype start";
    Game game;
    game.Run();
    BOOST_LOG_TRIVIAL(info) << "notype end";
    return 0;
}
