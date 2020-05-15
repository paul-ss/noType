#include "game.hpp"

#define GAME_NAME "noType"

Game::Game() {

    _clock.restart();
    srand(time(nullptr));

    _context = std::make_shared<SharedContext>();

    _window = std::make_shared<Window>(GAME_NAME);
    _context->window = _window;
    _context->eventManager = _window->GetEventManager();

    _audioManager = std::make_shared<AudioManager>();
    _context->audioManager = _audioManager;

    _soundManager = std::make_shared<SoundManager>(_context);
    _context->soundManager = _soundManager;

    _textureManager = std::make_shared<TextureManager>();
    _context->textureManager = _textureManager;

    _fontManager = std::make_shared<FontManager>();
    _fontManager->RequireResource("Main");
    _context->fontManager = _fontManager;

    _stateManager = std::make_shared<StateManager>(_context);
    _stateManager->SwitchTo(StateType::MainMenu);
}

sf::Time Game::getElapsed() {
    return _clock.getElapsedTime();
}

void Game::restartClock() {
    _elapsed = _clock.restart();
}

void Game::update() {
    _window->Update();
    _stateManager->Update(_elapsed);
    _soundManager->Update(_elapsed.asSeconds());
}

void Game::render() {
    _window->BeginDraw();
    _stateManager->Draw();
    _window->EndDraw();
}

void Game::lateUpdate() {
    _stateManager->ProcessRequests();
    restartClock();
}

void Game::Run() {
    while (!_window->IsDone()) {
        update();
        render();
        lateUpdate();
        sf::sleep(sf::milliseconds(0.01f));
    }
}
