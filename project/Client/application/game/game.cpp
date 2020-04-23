#include "game.hpp"

Game::Game() : _window("noType", sf::Vector2u(800,600)),
        _stateManager(&_context) {

    _clock.restart();
    srand(time(nullptr));

    _context._window = &_window;
    _context._eventManager = _window.getEventManager();

    _stateManager.switchTo(StateType::Intro);
}

sf::Time Game::getElapsed() {
    return _clock.getElapsedTime();
}

void Game::restartClock() {
    _elapsed = _clock.restart();
}

void Game::update() {
    _window.update();
    _stateManager.update(_elapsed);
}

void Game::render() {
    _window.beginDraw();
    _stateManager.draw();
    _window.endDraw();
}

void Game::lateUpdate() {
    _stateManager.processRequests();
    restartClock();
}

void Game::run() {
    while (!_window.isDone()) {
        update();
        render();
        lateUpdate();
        //sf::sleep(sf::seconds(0.2));
    }
}
