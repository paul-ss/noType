#include "introState.hpp"

IntroState::IntroState(std::shared_ptr<StateManager> state) : BaseState(state) {}
IntroState::~IntroState() {}

void IntroState::onCreate() {
    _timePassed = 0.0f;

    sf::Vector2u windowSize =
            _stateMgr->getContext()._window->getRenderWindow()->getSize();

    _introTexture.loadFromFile("intro.png");
    _introSprite.setTexture(_introTexture);
    _introSprite.setOrigin(_introTexture.getSize().x / 2.0f,
        _introTexture.getSize().y / 2.0f);

    _introSprite.setPosition(windowSize.x / 2.0f, 0);

    _font.loadFromFile("arial.ttf");
    _text.setFont(_font);
    _text.setString({ "Press SPACE to continue" });
    _text.setCharacterSize(15);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    auto evMgr = _stateMgr->getContext()._eventManager;
    auto lambdaContinue = 
            [this](EventDetails& details) {
                this->proceed(std::make_shared<EventDetails>(details));
            };

    evMgr->addCallback(StateType::Intro, "Intro_Continue", lambdaContinue);
}

void IntroState::onDestroy() {
    auto evMgr = _stateMgr->getContext()._eventManager;
    evMgr->removeCallback(StateType::Intro, "Intro_Continue");
}

void IntroState::update(const sf::Time& time) {
    if (_timePassed < 5.0f) {  // Less than five seconds.
        _timePassed += time.asSeconds();
        _introSprite.setPosition(
            _introSprite.getPosition().x,
            _introSprite.getPosition().y + (48 * time.asSeconds()));
    }
}

void IntroState::draw() {
    std::shared_ptr<sf::RenderWindow> window =
            _stateMgr->getContext()._window->getRenderWindow();

    window->draw(_introSprite);
    if(_timePassed >= 5.0f){
        window->draw(_text);
    }
}

void IntroState::proceed(std::shared_ptr<EventDetails> details) {
    if (_timePassed >= 5.0f) {
        _stateMgr->switchTo(StateType::MainMenu);
        _stateMgr->remove(StateType::Intro);
    }
}

void IntroState::activate() {}
void IntroState::deactivate() {}
