#include "game.hpp"

Game::Game() : _window("noType", sf::Vector2u(800,600)),
            _soundManager(&_audioManager),
            _guiManager(_window.GetEventManager(), &_context),
            _stateManager(&_context) {

    _clock.restart();
    srand(time(nullptr));

    _context._window = &_window;
    _context._eventManager = _window.GetEventManager();
    _context._audioManager = &_audioManager;
    _context._soundManager = &_soundManager;
    _context._textureManager = &_textureManager;
    _context._fontManager = &_fontManager;
    _context._guiManager = &_guiManager;

    _fontManager.RequireResource("Main");

    _stateManager.SwitchTo(StateType::Intro);
}

Game::~Game() {
    _fontManager.ReleaseResource("Main");
}

sf::Time Game::getElapsed() {
    return _clock.getElapsedTime();
}

void Game::restartClock() {
    _elapsed = _clock.restart();
}

void Game::update(){
    _window.Update();
    _stateManager.Update(_elapsed);
    _guiManager.Update(_elapsed.asSeconds());
    _soundManager.Update(_elapsed.asSeconds());

    GUI_Event guiEvent;
    while (_context._guiManager->PollEvent(guiEvent)){
        _window.GetEventManager()->HandleEvent(guiEvent);
    }
}

void Game::render(){
    _window.BeginDraw();
    _stateManager.Draw();

    _guiManager.Render(_window.GetRenderWindow());
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
    }
}
