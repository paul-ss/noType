#include "introState.hpp"
#include "stateManager.hpp"

IntroState::IntroState(StateManager* stateManager)
    : BaseState(stateManager) {}

IntroState::~IntroState() {}

void IntroState::OnCreate() {
    sf::Vector2u windowSize = _stateMgr->GetContext()->_window->GetRenderWindow()->getSize();

    TextureManager* textureMgr = _stateMgr->GetContext()->_textureManager;
    textureMgr->RequireResource("Intro");
    _introSprite.setTexture(*textureMgr->GetResource("Intro"));
    _introSprite.setOrigin(textureMgr->GetResource("Intro")->getSize().x / 2.0f,
                            textureMgr->GetResource("Intro")->getSize().y / 2.0f);

    _introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    _text.setString(sf::String("Press SPACE to continue"));
    _text.setCharacterSize(15);
    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
    _text.setPosition(_introSprite.getPosition().x, _introSprite.getPosition().y + textureMgr->GetResource("Intro")->getSize().y / 1.5f);

    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;

    auto lambdaContinue = [this](EventDetails& details) { this->Continue(&details); };
    evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

    _stateMgr->GetContext()->_soundManager->PlayMusic("noType", 100.f, true);
}

void IntroState::OnDestroy() {
    TextureManager* textureMgr = _stateMgr->GetContext()->_textureManager;
    textureMgr->ReleaseResource("Intro");

    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;
    evMgr->RemoveCallback(StateType::Intro,"Intro_Continue");
}

void IntroState::Draw() {
    sf::RenderWindow* window = _stateMgr->GetContext()->_window->GetRenderWindow();

    window->draw(_introSprite);
    window->draw(_text);
}

void IntroState::Continue(EventDetails* details) {
    _stateMgr->SwitchTo(StateType::MainMenu);
    _stateMgr->Remove(StateType::Intro);
}

void IntroState::Update(const sf::Time& time) {}
void IntroState::Activate() {}
void IntroState::Deactivate() {}