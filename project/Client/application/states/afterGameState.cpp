#include "afterGameState.hpp"
#include "sharedContext.hpp"
#include "label.hpp"
#include "textField.hpp"
#include "logger.hpp"

#define ELEM_MARGIN_Y 100

AfterGameState::AfterGameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void AfterGameState::OnCreate() {
    auto context = GetSharedContext();

    auto fillerItr = context->sharedElements.find(ElementName::Filler);
    if (fillerItr != context->sharedElements.end()) {
        _elements.emplace(fillerItr->first, fillerItr->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "filler not found";
    }
    auto positionItr = context->sharedElements.find(ElementName::PlayerPosition);
    if (positionItr != context->sharedElements.end()) {
        _elements.emplace(positionItr->first, positionItr->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "position not found";
    }
    auto aSpeedItr = context->sharedElements.find(ElementName::AverageSpeed);
    if (aSpeedItr != context->sharedElements.end()) {
        _elements.emplace(aSpeedItr->first, aSpeedItr->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "average speed not found";
    }

    auto renderWindow = GetRenderWindow();
    auto windowSize = renderWindow->getSize();
    auto menu = std::make_shared<Label>(ElementName::MenuButton, context, sf::Vector2f(0, 0), "button.json");
    auto menuSize = menu->GetSize();
    sf::Vector2f menuPosition((windowSize.x * 0.5f - windowSize.x * 0.5),
                              (windowSize.y * 0.5f - windowSize.y * 0.5) - ELEM_MARGIN_Y);
    menu->SetText("Exit");
    menu->SetPosition(menuPosition);
    _elements.emplace(ElementName::MenuButton, menu);

    auto eMgr = GetEventManager();
    auto lambdaClick = [this](EventDetails& l_details) { this->MouseClick(l_details); };
    eMgr->AddCallback(StateType::MainMenu, "Mouse_Left", lambdaClick);

    auto lambdaPlay = [this]([[maybe_unused]] EventDetails& l_details) { this->Menu(); };
    eMgr->AddCallback(StateType::MainMenu, "Intro_Continue", lambdaPlay);
}

void AfterGameState::MouseClick(EventDetails& l_details) {
    sf::Vector2i mousePos = l_details.mouse;
    for (auto& itr : _elements) {
        auto clicked = itr.second->OnClick(mousePos);

        switch (clicked) {
            case ElementName::MenuButton:
                Menu();
                break;

            default:
                break;
        }
    }
}

void AfterGameState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::PlayerPosition);
    drawElement(ElementName::AverageSpeed);
    drawElement(ElementName::MenuButton);
}

void AfterGameState::Menu() {
    try {
        auto stateMgr = GetStateManager();

        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::Intro);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - menu] " << e.what();
    }
}

void AfterGameState::OnDestroy() {
    try {
        auto eMgr = GetEventManager();

        eMgr->RemoveCallback(StateType::MainMenu, "Key_Enter");
        eMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - ondestroy] " << e.what();
    }
}

void AfterGameState::Activate() {}
void AfterGameState::Deactivate() {}

void AfterGameState::Update(const sf::Time& l_time) {}
