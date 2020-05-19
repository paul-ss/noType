#include "game.hpp"

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

    //_soundManager = std::make_shared<SoundManager>(_context, MUSIC_VOLUME);
    //_context->soundManager = _soundManager;

    _textureManager = std::make_shared<TextureManager>();
    _context->textureManager = _textureManager;

    _fontManager = std::make_shared<FontManager>();
    _fontManager->RequireResource("Main");
    _context->fontManager = _fontManager;

    // NOTE(vendroid): Надо было делать сначала ОТДЕЛЬНЫЙ объект типа std::shared_ptr<Network::QueueManager>
    //                 с помощью std::make_shared<Network::QueueManager>(), а потом уже им инициализировать
    //                 поле класса и передавать НЕ ПОЛЕ, а объект класса :)
    //                 Я сам обосрался в контруктуре NetworkManager'a -- написал там шаренный указатель
    //                 по ссылке константной ссылке на Connector::IQueueManager. Конечно же он должен
    //                 принимать обычный указатель, что бы прокнул полиморфизм.
    // -------------->*
    auto queueManager = std::make_shared<Network::QueueManager>();
    _queueManager = queueManager;

    _networkManager = std::make_shared<Network::NetworkManager>(queueManager);

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
    //_soundManager->Update(_elapsed);
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
    _networkManager->Connect();
    _networkManager->Run();
    while (!_window->IsDone()) {
        update();
        render();
        lateUpdate();
        //sf::sleep(sf::milliseconds(500));
    }
    _networkManager->Disconnect();
}
