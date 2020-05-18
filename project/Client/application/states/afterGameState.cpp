#include "afterGameState.hpp"

AfterGameState::AfterGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

AfterGameState::~AfterGameState() {}

    void AfterGameState::OnCreate() {}
    void AfterGameState::OnDestroy() {}

    void AfterGameState::Activate() {}
    void AfterGameState::Deactivate() {}

    void AfterGameState::Update(const sf::Time& l_time) {}
    void AfterGameState::Draw() {
        try {
            std::shared_ptr<StateManager>stateMgr(_stateMgr);
            std::shared_ptr<SharedContext>context(stateMgr->GetContext());
            std::shared_ptr<EventManager>eMgr(context->eventManager);
            std::shared_ptr<Window>window(context->window);
            std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
            sf::RectangleShape test;
            test.setSize(sf::Vector2f(355, 355));
            test.setFillColor(sf::Color::Magenta);
            renderWindow->draw(test);
        } catch (...) {
        }
    }