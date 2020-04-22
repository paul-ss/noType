#include "fileLogger.hpp"
#include "game.hpp"

int main() {
    FileLogger::getInstance();
    Game game;
    game.run();
    return 0;
}
