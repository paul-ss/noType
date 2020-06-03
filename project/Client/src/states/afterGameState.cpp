#include "afterGameState.hpp"
#include "sharedContext.hpp"
#include "label.hpp"
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
    auto positionTextItr = context->sharedElements.find(ElementName::PlayerPositionText);
    if (positionTextItr != context->sharedElements.end()) {
        _elements.emplace(positionTextItr->first, positionTextItr->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "positiontext not found";
    }

    auto aSpeedItr = context->sharedElements.find(ElementName::AverageSpeed);
    if (aSpeedItr != context->sharedElements.end()) {
        _elements.emplace(aSpeedItr->first, aSpeedItr->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "average speed not found";
    }
    auto aSpeedItrTxt = context->sharedElements.find(ElementName::AverageSpeedText);
    if (aSpeedItrTxt != context->sharedElements.end()) {
        _elements.emplace(aSpeedItrTxt->first, aSpeedItrTxt->second);
    } else {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - oncreate] " << "average speed not found";
    }

    auto menu = std::make_shared<Label>(ElementName::MenuButton, context, "button.json");
    menu->SetText("Menu");
    _elements.emplace(ElementName::MenuButton, menu);

    auto eMgr = GetEventManager();
    auto lambdaClick = [this](const EventDetails& l_details) { this->MouseClick(l_details); };
    eMgr->AddCallback(StateType::AfterGame, "Mouse_Left", lambdaClick);

    auto lambdaPlay = [this]([[maybe_unused]] const EventDetails& l_details) { this->Menu(); };
    eMgr->AddCallback(StateType::AfterGame, "Key_Enter", lambdaPlay);
}

void AfterGameState::MouseClick(const EventDetails& l_details) {
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
    drawElement(ElementName::PlayerPositionText);
    drawElement(ElementName::AverageSpeed);
    drawElement(ElementName::AverageSpeedText);
    drawElement(ElementName::MenuButton);
}

void AfterGameState::clearButton() {
    auto menuItr = _elements.find(ElementName::MenuButton);
    if (menuItr == _elements.end()) {
        return;
    }
    menuItr->second->OnLeave();
}

void AfterGameState::Menu() {
    try {
        auto stateMgr = GetStateManager();

        clearButton();
        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::AfterGame);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[aftergamestate - menu] " << e.what();
    }
}

void AfterGameState::OnDestroy() {
    try {
        auto eMgr = GetEventManager();

        eMgr->RemoveCallback(StateType::AfterGame, "Key_Enter");
        eMgr->RemoveCallback(StateType::AfterGame, "Mouse_Left");

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - ondestroy] " << e.what();
    }
}

void AfterGameState::Update(const sf::Time& l_time) {
    auto playItr = _elements.find(ElementName::MenuButton);
    playItr->second->Update(l_time.asSeconds());
}

void AfterGameState::Activate() {}
void AfterGameState::Deactivate() {}
