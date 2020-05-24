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
        auto renderWindow = GetRenderWindow();

        auto itr = context->sharedElements.find(ElementName::Filler);
        if (itr != context->sharedElements.end()) {
            _elements.emplace(itr->first, itr->second);
        } else {
            BOOST_LOG_TRIVIAL(error) << "[menu - oncreate] " << "Filler not found";
        }

        auto windowSize = renderWindow->getSize();
        auto play = std::make_shared<Label>(ElementName::PlayButton, context, sf::Vector2f(0, 0), "button.json");
        auto playSize = play->GetSize();
        sf::Vector2f playPosition((windowSize.x * 0.5f - playSize.x * 0.5),
                (windowSize.y * 0.5f - playSize.y * 0.5) - ELEM_MARGIN_Y);
        play->SetText("Play");
        play->SetPosition(playPosition);
        _elements.emplace(ElementName::PlayButton, play);

        auto quit = std::make_shared<Label>(ElementName::QuitButton, context, sf::Vector2f(0, 0), "button.json");
        auto quitSize = quit->GetSize();
        sf::Vector2f quitPosition((windowSize.x * 0.5f - quitSize.x * 0.5),
                (windowSize.y * 0.5f - quitSize.y * 0.5) + ELEM_MARGIN_Y);
        quit->SetText("Quit");
        quit->SetPosition(quitPosition);
        _elements.emplace(ElementName::QuitButton, quit);

        auto eMgr = GetEventManager();
        auto lambdaQuit = [this]([[maybe_unused]] EventDetails& l_details) { this->Quit(); };
        eMgr->AddCallback(StateType::MainMenu, "Key_Escape", lambdaQuit);

        auto lambdaClick = [this](EventDetails& l_details) { this->MouseClick(l_details); };
        eMgr->AddCallback(StateType::MainMenu, "Mouse_Left", lambdaClick);

        auto lambdaPlay = [this]([[maybe_unused]] EventDetails& l_details) { this->Play(); };
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

void MainMenuState::MouseClick(EventDetails& l_details) {
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

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - play] " << e.what();
    }
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
void MainMenuState::Update([[maybe_unused]] const sf::Time& dT) {}
void MainMenuState::Deactivate() {}
