#include "introState.hpp"
#include "sharedContext.hpp"
#include "filler.hpp"
#include "logger.hpp"

IntroState::IntroState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void IntroState::OnCreate() {
    try {
        auto context = GetSharedContext();
        auto renderWindow = GetRenderWindow();

        auto windowSize = renderWindow->getSize();
        sf::Vector2f windowCenter(windowSize.x * 0.5, windowSize.y * 0.5);
        auto filler = std::make_shared<Filler>(ElementName::Filler, context, windowCenter, "filler.json");
        sf::Vector2f fillerPosition(windowSize.x * 0.5f - filler->GetSize().x * 0.5,
                windowSize.y * 0.5f);
        filler->SetPosition(fillerPosition);
        context->sharedElements.emplace(ElementName::Filler, filler);
        _elements.emplace(ElementName::Filler, filler);

        auto introSprite = std::make_shared<Filler>(ElementName::IntroSprite, context, windowCenter, "introSprite.json");
        introSprite->SetText("PRESS SPACE TO CONTINUE");
        _elements.emplace(ElementName::IntroSprite, introSprite);

        auto eMgr = GetEventManager();
        auto lambdaContinue = [this](EventDetails& details) { this->Continue(details); };
        eMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - oncreate] " << e.what();
        return;
    }
}

void IntroState::OnDestroy() {
    try {
        auto eMgr = GetEventManager();
        eMgr->RemoveCallback(StateType::Intro, "Intro_Continue");

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - ondestroy] " << e.what();
    }
}

void IntroState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::IntroSprite);
}

void IntroState::Continue([[maybe_unused]] EventDetails& l_details) {
    try {
        auto stateMgr = GetStateManager();

        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::Intro);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - continue] " << e.what();
    }
}

void IntroState::Update([[maybe_unused]] const sf::Time& time) {}
void IntroState::Activate() {}
void IntroState::Deactivate() {}
