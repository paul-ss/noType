#include "gameState.hpp"
#include "stateManager.hpp"

GameState::GameState(StateManager* stateManager)
    : BaseState(stateManager) {}

GameState::~GameState() {}

void GameState::OnCreate() {
    _texture.loadFromFile("Mushroom.png");
    _sprite.setTexture(_texture);
    _sprite.setPosition(0,0);
    _increment = sf::Vector2f(400.0f, 400.0f);

    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    //evMgr->addCallback(StateType::Game,"Key_Escape",&GameState::MainMenu,this);
    //evMgr->addCallback(StateType::Game,"Key_P",&GameState::Pause,this);
}

void GameState::OnDestroy() {
    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    evMgr->removeCallback(StateType::Game,"Key_Escape");
    evMgr->removeCallback(StateType::Game,"Key_P");
}

void GameState::Update(const sf::Time& time) {

}

void GameState::Draw() {
    _stateMgr->getContext()->_window->GetRenderWindow()->draw(_sprite);
}

void GameState::MainMenu(EventDetails* details) {
    _stateMgr->switchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details) {
    _stateMgr->switchTo(StateType::Paused);
}

void GameState::Activate() {}
void GameState::Deactivate() {}
