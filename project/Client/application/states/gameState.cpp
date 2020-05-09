#include "gameState.hpp"

GameState::GameState(std::weak_ptr<StateManager> stateManager)
    : BaseState(stateManager) {}

GameState::~GameState() {}

void GameState::OnCreate() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [game - ondestroy]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [game - oncreate]";
        return;
    }
    auto eMgr = context->_eventManager.lock();
    if (!eMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid event manager [game - oncreate]";
        return;
    }
    auto lambdaMainMenu = [this](EventDetails& details) { this->MainMenu(details); };
    eMgr->AddCallback(StateType::Game, "Key_Escape", lambdaMainMenu);
}

void GameState::OnDestroy() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [game - ondestroy]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [game - ondestroy]";
        return;
    }
    auto eMgr = context->_eventManager.lock();
    if (!eMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid event manager [game - ondestroy]";
        return;
    }
    eMgr->RemoveCallback(StateType::Game,"Key_Escape");
}

void GameState::Update(const sf::Time& time) {
}

void GameState::Draw() {
}

void GameState::MainMenu(EventDetails& details) {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [game - mainmenu]";
        return;
    }
    stateMgr->SwitchTo(StateType::MainMenu);
}

void GameState::Activate() {}
void GameState::Deactivate() {}
