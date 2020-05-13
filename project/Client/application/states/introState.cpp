#include "introState.hpp"

IntroState::IntroState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

void IntroState::OnCreate() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->_window);

        std::shared_ptr<TextureManager>textureMgr(context->_textureManager);
        textureMgr->RequireResource("Intro");

        std::shared_ptr<sf::Texture>introTexture(textureMgr->GetResource("Intro"));
        _introSprite.setTexture(*introTexture);
        _introSprite.setOrigin(introTexture->getSize().x / 2.0f,
                                introTexture->getSize().y / 2.0f);

        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        std::cout << renderWindow.use_count() <<"<- on create renderWindow \n";
        sf::Vector2u windowSize = renderWindow->getSize();
        _introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

        sf::FloatRect textRect = _text.getLocalBounds();
        std::shared_ptr<FontManager>fontMgr(context->_fontManager);
        std::shared_ptr<sf::Font>mainFont(fontMgr->GetResource("Main"));
        _text.setFont(*mainFont);
        _text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
        _text.setString({ "Press SPACE to continue" });
        _text.setFillColor(sf::Color::Cyan);
        _text.setCharacterSize(20);

        std::shared_ptr<EventManager>evMgr(context->_eventManager);
        auto lambdaContinue = [this](EventDetails& details) { this->Continue(details); };
        evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

        std::shared_ptr<SoundManager>soundMgr(context->_soundManager);
        //soundMgr->PlayMusic("noType", 50.f, true);
    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - oncreate] " << e.what();
        return;
    }
}

void IntroState::OnDestroy() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());

        std::shared_ptr<EventManager>evMgr(context->_eventManager);
        evMgr->RemoveCallback(StateType::Intro,"Intro_Continue");
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - ondestroy] " << e.what();
    }
}

void IntroState::Draw() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->_window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        renderWindow->draw(_introSprite);
        renderWindow->draw(_text);
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - draw] " << e.what();
    }
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