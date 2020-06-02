#include "game.hpp"
#include "sharedContext.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#define GAME_NAME "noType"
#define MUSIC_VOLUME 50.0f

Game::Game() {

    _clock.restart();
    srand(time(nullptr));

    _context = std::make_shared<SharedContext>();

    _window = std::make_shared<Window>(GAME_NAME);
    _context->window = _window;
    _context->eventManager = _window->GetEventManager();

    _audioManager = std::make_shared<AudioManager>();
    _context->audioManager = _audioManager;

    _soundManager = std::make_shared<SoundManager>(_context, MUSIC_VOLUME);
    _context->soundManager = _soundManager;

    _textureManager = std::make_shared<TextureManager>();
    _context->textureManager = _textureManager;

    _fontManager = std::make_shared<FontManager>();
    _context->fontManager = _fontManager;

    auto queueManager = std::make_shared<Network::QueueManager>();
    _queueManager = queueManager;
    _context->queueManager = _queueManager;


    _networkManager = std::make_shared<Network::NetworkManager>(queueManager);
    _context->networkManager = _networkManager;

    _stateManager = std::make_shared<StateManager>(_context);
    _context->stateManager = _stateManager;

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
    _soundManager->Update(_elapsed);
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
    try {
        _networkManager->Connect();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[run - game] " << e.what();
        return;
    }

    _networkManager->Run();
    while (!_window->IsDone()) {
        update();
        render();
        lateUpdate();
    }
    _networkManager->Disconnect();
    _networkManager->Stop();
}
