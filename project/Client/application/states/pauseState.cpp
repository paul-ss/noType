#include "pauseState.hpp"
#include "stateManager.hpp"

PauseState::PauseState(StateManager* stateManager)
    : BaseState(stateManager) {}

PauseState::~PauseState() {}

void PauseState::onCreate() {
    setTransparent(true);
    _font.loadFromFile("resources/arcade.ttf");
    _text.setFont(_font);
    _text.setString(sf::String("PAUSED"));
    _text.setCharacterSize(14);
    _text.setStyle(sf::Text::Bold);

    sf::Vector2u windowSize = _stateMgr->getContext()->_window->getRenderWindow()->getSize();

    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    _rect.setSize(sf::Vector2f(windowSize));
    _rect.setPosition(0,0);
    _rect.setFillColor(sf::Color(0, 0, 0, 150));

    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    auto lambdaPause =
            [this](EventDetails& details) {
                this->unpause(&details);
            };
    evMgr->addCallback(StateType::Paused, "Key_P", lambdaPause);
}

void PauseState::onDestroy() {
    EventManager* evMgr = _stateMgr->getContext()->_eventManager;
    evMgr->removeCallback(StateType::Paused, "Key_P");
}

void PauseState::draw() {
    sf::RenderWindow* window = _stateMgr->getContext()->_window->getRenderWindow();
    window->draw(_rect);
    window->draw(_text);
}

void PauseState::unpause(EventDetails* details) {
    _stateMgr->switchTo(StateType::Game); 
}

void PauseState::activate() {}
void PauseState::deactivate() {}
void PauseState::update(const sf::Time& time) {}
