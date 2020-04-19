#include <game.hpp>

int main() {
    Game& game = Game::getInstance();
    game.run();
    return 0;
}
