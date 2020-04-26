#include "introState.hpp"

IntroState::IntroState(StateManager* state) : BaseState(state) {}
IntroState::~IntroState() {}

void IntroState::OnCreate() {
    _timePassed = 0.0f;

    sf::Vector2u windowSize =
            _stateMgr->GetContext()->_window->GetRenderWindow()->getSize();

    _introTexture.loadFromFile("resources/media/textures/rose.png");
    _introSprite.setTexture(_introTexture);
    _introSprite.setOrigin(_introTexture.getSize().x / 2.0f,
        _introTexture.getSize().y / 2.0f);

    _introSprite.setPosition(windowSize.x / 2.0f, 0);

    _font.loadFromFile("resources/media/fonts/arcade.ttf");
    _text.setFont(_font);
    _text.setString({ "Press SPACE to continue" });
    _text.setCharacterSize(15);
    _text.setColor(sf::Color::Cyan);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    auto evMgr = _stateMgr->GetContext()->_eventManager;
    auto lambdaContinue = 
            [this](EventDetails& details) {
                this->Continue(&details);
            };
    evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);
    _stateMgr->GetContext()->_soundManager->PlayMusic("noType");
}

void IntroState::OnDestroy() {
    auto evMgr = _stateMgr->GetContext()->_eventManager;
    evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void IntroState::Update(const sf::Time& time) {
    if (_timePassed < 5.0f) {  // Less than five seconds.
        _timePassed += time.asSeconds();
        _introSprite.setPosition(
            _introSprite.getPosition().x,
            _introSprite.getPosition().y + (48 * time.asSeconds()));
    }
}

void IntroState::Draw() {
    sf::RenderWindow* window =
            _stateMgr->GetContext()->_window->GetRenderWindow();

    window->draw(_introSprite);
    if(_timePassed >= 5.0f){
        window->draw(_text);
    }
}

void IntroState::Continue(EventDetails* details) {
    if (_timePassed >= 5.0f) {
        _stateMgr->SwitchTo(StateType::MainMenu);
        _stateMgr->Remove(StateType::Intro);
    }
}

void IntroState::Activate() {}
void IntroState::Deactivate() {}
