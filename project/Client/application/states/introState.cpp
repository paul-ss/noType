#include "introState.hpp"
#include "utils.hpp"

IntroState::IntroState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

void IntroState::OnCreate() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        auto windowSize = renderWindow->getSize();
        auto filler = std::make_shared<Label>(context, sf::Vector2f(0, 0), "filler.json");
        sf::Vector2f fillerPosition(windowSize.x * 0.5f - filler->GetSize().x * 0.5,
                windowSize.y * 0.5f);
        filler->SetPosition(fillerPosition);
        context->sharedElements.emplace("Filler", filler);
        _elements.emplace("Filler", filler);

        auto introSprite = std::make_shared<Label>(context, sf::Vector2f(0, 0), "introSprite.json");
        sf::Vector2f introPosition(windowSize.x * 0.5f - introSprite->GetSize().x * 0.5,
                windowSize.y * 0.5f);
        introSprite->SetPosition(introPosition);
        introSprite->SetText("PRESS SPACE TO CONTINUE");
        _elements.emplace("IntroSprite", introSprite);

        std::shared_ptr<EventManager>evMgr(context->eventManager);
        auto lambdaContinue = [this](EventDetails& details) { this->Continue(details); };
        evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - oncreate] " << e.what();
        return;
    }
}

void IntroState::OnDestroy() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());

        std::shared_ptr<EventManager>evMgr(context->eventManager);
        evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - ondestroy] " << e.what();
    }
}

void IntroState::Draw() {
    for (auto& element : _elements) {
        element.second->Draw();
    }
}

void IntroState::Continue(EventDetails& l_details) {
    utils::unusedArgs(l_details);
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [intro - continue]";
        return;
    }
    stateMgr->SwitchTo(StateType::MainMenu);
    stateMgr->Remove(StateType::Intro);
}

void IntroState::Update(const sf::Time& time) {}

void IntroState::Activate() {}
void IntroState::Deactivate() {}
