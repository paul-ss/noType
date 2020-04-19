#include "game.hpp"

Game::Game() : _window("noType", sf::Vector2u(800,600)) {
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

void Game::run() {
    while (!_window.isDone()) {
        update();
        render();
        sf::sleep(sf::seconds(0.2));
    }
}
