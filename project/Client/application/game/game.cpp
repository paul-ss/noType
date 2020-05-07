#include "game.hpp"

Game::Game() : _window("noType", sf::Vector2u(800,600)) {
}

Game& Game::getInstance() {
    static Game rValue;
    return rValue;
}

void Game::update() {
    _window.update();
    //game logic
}

void Game::render() {
    _window.beginDraw();
    //_window.draw();
    _window.endDraw();
}

sf::Time Game::getElapsed() {
    return _elapsed;
}

void Game::restartClock() {
    _elapsed += _clock.restart();
}

bool Game::run() {
    float frametime = 1.0f / 60.0f;
    while (!_window.isDone()) {
        if(_elapsed.asSeconds() >= frametime) {
            handleInput();
            update();
            render();
            _elapsed -= sf::seconds(frametime);
        }
        restartClock();
    }
}
