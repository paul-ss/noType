#include "game.hpp"

#define GAME_NAME "noType"
#define WINDOW_HEIGHT 800
#define WINDOW_WEIGHT 600

Game::Game() {

    _clock.restart();
    srand(time(nullptr));

    _context = std::make_shared<SharedContext>();

    _window = std::make_shared<Window>(GAME_NAME, sf::Vector2u(WINDOW_HEIGHT, WINDOW_WEIGHT));
    _context->_window = _window;
    _context->_eventManager = _window->GetEventManager();

    _audioManager = std::make_shared<AudioManager>();
    _context->_audioManager = _audioManager;

    _soundManager = std::make_shared<SoundManager>(_context->_audioManager);
    _context->_soundManager = _soundManager;

    _textureManager = std::make_shared<TextureManager>();
    _context->_textureManager = _textureManager;

    _fontManager = std::make_shared<FontManager>();
    _fontManager->RequireResource("Main");
    _context->_fontManager = _fontManager;

    //_guiManager = std::make_shared<GUI_Manager>(_window->GetEventManager(), _context);
    //_context->_guiManager = _guiManager;

    _stateManager = std::make_shared<StateManager>(_context);
    _stateManager->SwitchTo(StateType::Intro);
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
    //_guiManager->Update(_elapsed.asSeconds());
    _soundManager->Update(_elapsed.asSeconds());

    //GUI_Event guiEvent;
    //while (_context->_guiManager.lock()->PollEvent(guiEvent)) {
    //    _window->GetEventManager().lock()->HandleEvent(guiEvent);
    //}
}

void Game::render() {
    _window->BeginDraw();
    _stateManager->Draw();

    //_guiManager->Render(_window->GetRenderWindow());
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
        sf::sleep(sf::seconds(0.01f));
    }
}
