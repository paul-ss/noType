#include "mainMenuState.hpp"

MainMenuState::MainMenuState(StateManager* stateManager)
    : BaseState(stateManager) {}

MainMenuState::~MainMenuState() {}

void MainMenuState::OnCreate() {
    GUI_Manager* gui = _stateMgr->GetContext()->_guiManager;
    gui->LoadInterface(StateType::MainMenu, "MainMenu.interface", "MainMenu");
    gui->GetInterface(StateType::MainMenu, "MainMenu")->SetPosition(sf::Vector2f(250.f, 168.f));

    EventManager* eMgr = _stateMgr->GetContext()->_eventManager;
    auto lambdaPlay = [this](EventDetails& details) { this->Play(&details); };
    eMgr->AddCallback(StateType::MainMenu, "MainMenu_Play", lambdaPlay);
    auto lambdaQuit = [this](EventDetails& details) { this->Quit(&details); };
    eMgr->AddCallback(StateType::MainMenu, "MainMenu_Quit", lambdaQuit);
}

void MainMenuState::OnDestroy() {
    _stateMgr->GetContext()->_guiManager->RemoveInterface(StateType::MainMenu, "MainMenu");

    EventManager* eMgr = _stateMgr->GetContext()->_eventManager;
    eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
    eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
}

void MainMenuState::Activate() {
    auto& play = *_stateMgr->GetContext()->_guiManager->
        GetInterface(StateType::MainMenu, "MainMenu")->GetElement("Play");
    if (_stateMgr->HasState(StateType::Game)) {
        // Resume
        play.SetText("Resume");
    } else {
        // Play
        play.SetText("Play");
    }
}

void MainMenuState::Play(EventDetails* details) {
    _stateMgr->SwitchTo(StateType::Game);
}

void MainMenuState::Quit(EventDetails* details) {
    _stateMgr->GetContext()->_window->Close();
}

void MainMenuState::Draw() {}
void MainMenuState::Update(const sf::Time& dT) {}
void MainMenuState::Deactivate() {}
