#include "gameState.hpp"
#include "stateManager.hpp"

GameState::GameState(StateManager* stateManager)
    : BaseState(stateManager) {}

GameState::~GameState() {}

void GameState::onCreate() {
    _texture.loadFromFile("Mushroom.png");
    _sprite.setTexture(_texture);
    _sprite.setPosition(0,0);
    _increment = sf::Vector2f(400.0f, 400.0f);

    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    //evMgr->addCallback(StateType::Game,"Key_Escape",&GameState::MainMenu,this);
    //evMgr->addCallback(StateType::Game,"Key_P",&GameState::Pause,this);
}

void GameState::onDestroy() {
    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    evMgr->removeCallback(StateType::Game,"Key_Escape");
    evMgr->removeCallback(StateType::Game,"Key_P");
}

void GameState::update(const sf::Time& time) {

}

void GameState::draw() {
    _stateMgr->getContext()->_window->getRenderWindow()->draw(_sprite);
}

void GameState::mainMenu(EventDetails* details) {
    _stateMgr->switchTo(StateType::MainMenu);
}

void GameState::pause(EventDetails* details) {
    _stateMgr->switchTo(StateType::Paused);
}

void GameState::activate() {}
void GameState::deactivate() {}
