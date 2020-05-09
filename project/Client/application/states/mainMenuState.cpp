#include "mainMenuState.hpp"
#include "logger.hpp"

MainMenuState::MainMenuState(std::weak_ptr<StateManager> stateManager)
    : BaseState(stateManager) {}

void MainMenuState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<GUI_Manager> gui(context->_guiManager);
        gui->LoadInterface(StateType::MainMenu, "mainMenu.interface", "MainMenu");

        std::shared_ptr<GUI_Interface>interface(gui->GetInterface(StateType::MainMenu, "MainMenu"));
        interface->SetPosition(sf::Vector2f(250.f, 168.f));

        std::shared_ptr<EventManager> eMgr(context->_eventManager);
        auto lambdaPlay = [this](EventDetails& details) { this->Play(details); };
        eMgr->AddCallback(StateType::MainMenu, "MainMenu_Play", lambdaPlay);
        auto lambdaQuit = [this](EventDetails& details) { this->Quit(details); };
        eMgr->AddCallback(StateType::MainMenu, "MainMenu_Quit", lambdaQuit);

    } catch (const std::bad_weak_ptr &e) {
        // TODO(vendroid): исключение тоже в лог?
        BOOST_LOG_TRIVIAL(error) << "[menu - oncreate] " << e.what();
        return;
    }
}

void MainMenuState::OnDestroy() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - ondestroy]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [menu - ondestroy]";
        return;
    }

    auto gui = context->_guiManager.lock();
    if (!gui) {
        BOOST_LOG_TRIVIAL(error) << "Not valid gui manager [menu - ondestroy]";
        return;
    }
    gui->RemoveInterface(StateType::MainMenu, "MainMenu");

    auto eMgr = context->_eventManager.lock();
    if (!eMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid event manager [menu - oncreate]";
        return;
    }
    eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Play");
    eMgr->RemoveCallback(StateType::MainMenu, "MainMenu_Quit");
}

void MainMenuState::Play(EventDetails& details) {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - play]";
        return;
    }
    stateMgr->SwitchTo(StateType::Game);
}

void MainMenuState::Quit(EventDetails& details) {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - quit]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [menu - quit]";
        return;
    }
    auto window = context->_window.lock();
    if (!window) {
        BOOST_LOG_TRIVIAL(error) << "Not valid window [menu - quit]";
        return;
    }
    window->Close();
}

void MainMenuState::Activate() {}
void MainMenuState::Draw() {}
void MainMenuState::Update(const sf::Time& dT) {}
void MainMenuState::Deactivate() {}
