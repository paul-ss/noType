#include "mainMenuState.hpp"
#include "sharedContext.hpp"
#include "label.hpp"
#include "logger.hpp"

#define ELEM_MARGIN_Y 100

MainMenuState::MainMenuState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void MainMenuState::OnCreate() {
    try {
        auto context = GetSharedContext();

        auto itr = context->sharedElements.find(ElementName::Filler);
        if (itr != context->sharedElements.end()) {
            _elements.emplace(itr->first, itr->second);
        } else {
            BOOST_LOG_TRIVIAL(error) << "[menu - oncreate] " << "Filler not found";
        }

        auto play = std::make_shared<Label>(ElementName::PlayButton, context, "button.json");
        play->SetText("Play");
        _elements.emplace(ElementName::PlayButton, play);

        auto quit = std::make_shared<Label>(ElementName::QuitButton, context, "button.json");
        quit->SetText("Quit");
        _elements.emplace(ElementName::QuitButton, quit);

        auto eMgr = GetEventManager();
        auto lambdaQuit = [this]([[maybe_unused]] const EventDetails& l_details) { this->Quit(); };
        eMgr->AddCallback(StateType::MainMenu, "Key_Escape", lambdaQuit);

        auto lambdaClick = [this](const EventDetails& l_details) { this->MouseClick(l_details); };
        eMgr->AddCallback(StateType::MainMenu, "Mouse_Left", lambdaClick);

        auto lambdaPlay = [this]([[maybe_unused]] const EventDetails& l_details) { this->Play(); };
        eMgr->AddCallback(StateType::MainMenu, "Key_Enter", lambdaPlay);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - oncreate] " << e.what();
    }
}

void MainMenuState::OnDestroy() {
    try {
        auto eMgr = GetEventManager();

        eMgr->RemoveCallback(StateType::MainMenu, "Key_Escape");
        eMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
        eMgr->RemoveCallback(StateType::MainMenu, "Text_Entered");

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - ondestroy] " << e.what();
    }
}

void MainMenuState::MouseClick(const EventDetails& l_details) {
    sf::Vector2i mousePos = l_details.mouse;
    for (auto& itr : _elements) {
        auto clicked = itr.second->OnClick(mousePos);

        switch (clicked) {
        case ElementName::PlayButton:
            Play();
            break;

        case ElementName::QuitButton:
            Quit();
            break;

        default:
            break;
        }
    }
}

void MainMenuState::Play() {
    try {
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::BeforeGame);
        clearButton();

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - play] " << e.what();
    }
}

void MainMenuState::Update([[maybe_unused]] const sf::Time& l_dT) {
    auto playItr = _elements.find(ElementName::PlayButton);
    playItr->second->Update(l_dT.asSeconds());
    auto quitItr = _elements.find(ElementName::QuitButton);
    quitItr->second->Update(l_dT.asSeconds());
}

void MainMenuState::clearButton() {
    auto playItr = _elements.find(ElementName::PlayButton);
    if (playItr == _elements.end()) {
        return;
    }
    playItr->second->OnLeave();
    auto quitItr = _elements.find(ElementName::QuitButton);
    if (quitItr == _elements.end()) {
        return;
    }
    quitItr->second->OnLeave();
}

void MainMenuState::Quit() {
    try {
        auto window = GetWindow();
        window->Close();

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - quit] " << e.what();
    }
}

void MainMenuState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::PlayButton);
    drawElement(ElementName::QuitButton);
}

void MainMenuState::Activate() {}
void MainMenuState::Deactivate() {}
