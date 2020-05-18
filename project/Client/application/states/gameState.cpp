#include "gameState.hpp"
#include "stateManager.hpp"

GameState::GameState(StateManager* stateManager)
    : BaseState(stateManager) {}

GameState::~GameState() {}

void GameState::OnCreate() {

    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;
    auto lambdaMainMenu = [this](EventDetails& details) { this->MainMenu(&details); };
    evMgr->AddCallback(StateType::Game,"Key_Escape", lambdaMainMenu);
}

void GameState::OnDestroy() {
    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;
    evMgr->RemoveCallback(StateType::Game,"Key_Escape");
}

void GameState::Update(const sf::Time& time) {
}

void GameState::Draw() {
}

void GameState::MainMenu(EventDetails* details) {
    _stateMgr->SwitchTo(StateType::MainMenu);
}

void GameState::Activate() {}
void GameState::Deactivate() {}
