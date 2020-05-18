#include "game.hpp"

Game::Game() : _window("noType", sf::Vector2u(800,600)), _context{},
        _stateManager(&_context), _audioManager{}, _soundManager(&_audioManager) {

    _clock.restart();
    srand(time(nullptr));

    _context._window = &_window;
    _context._eventManager = _window.GetEventManager();
    _context._audioManager = &_audioManager;
    _context._soundManager = &_soundManager;

    _stateManager.SwitchTo(StateType::Intro);
}

sf::Time Game::getElapsed() {
    return _clock.getElapsedTime();
}

void Game::restartClock() {
    _elapsed = _clock.restart();
}

void Game::update() {
    _window.Update();
    _stateManager.Update(_elapsed);
    _soundManager.Update(_elapsed.asSeconds());
}

void Game::render() {
    _window.BeginDraw();
    _stateManager.Draw();
    _window.EndDraw();
}

void Game::lateUpdate() {
    _stateManager.ProcessRequests();
    restartClock();
}

void Game::Run() {
    while (!_window.IsDone()) {
        update();
        render();
        lateUpdate();
        //sf::sleep(sf::seconds(0.2));
    }
}
