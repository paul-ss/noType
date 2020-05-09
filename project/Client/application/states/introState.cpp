#include "introState.hpp"

IntroState::IntroState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

void IntroState::OnCreate() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - quit]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [menu - oncreate]";
        return;
    }
    auto window = context->_window.lock();
    if (!window) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [menu - oncreate]";
        return;
    }
    sf::Vector2u windowSize = stateMgr->GetContext()->_window->GetRenderWindow()->getSize();

    auto textureMgr = _stateMgr->GetContext()->_textureManager.lock();
    textureMgr->RequireResource("Intro");
    _introSprite.setTexture(*textureMgr->GetResource("Intro"));
    _introSprite.setOrigin(textureMgr->GetResource("Intro")->getSize().x / 2.0f,
                            textureMgr->GetResource("Intro")->getSize().y / 2.0f);

    _introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

    sf::FloatRect textRect = _text.getLocalBounds();
    auto fontMgr = _stateMgr->GetContext()->_fontManager.lock();

    _text.setFont(*fontMgr->GetResource("Main"));
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    _text.setString({ "Press SPACE to continue" });
    _text.setColor(sf::Color::Cyan);
    _text.setCharacterSize(20);

    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;

    auto lambdaContinue = [this](EventDetails& details) { this->Continue(&details); };
    evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

    _stateMgr->GetContext()->_soundManager->PlayMusic("noType", 100.f, true);
}

void IntroState::OnDestroy() {
    auto textureMgr = _stateMgr->GetContext()->_textureManager.lock();
    textureMgr->ReleaseResource("Intro");

    auto fontMgr = _stateMgr->GetContext()->_fontManager.lock();
    fontMgr->ReleaseResource("Main");

    auto evMgr = _stateMgr->GetContext()->_eventManager.lock();
    evMgr->RemoveCallback(StateType::Intro,"Intro_Continue");
}

void IntroState::Draw() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [intro - continue]";
        return;
    }
    sf::RenderWindow* window = stateMgr->GetContext()->_window->GetRenderWindow();

    window->draw(_introSprite);
    window->draw(_text);
}

void IntroState::Continue(EventDetails& details) {
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