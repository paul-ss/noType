#include "introState.hpp"

IntroState::IntroState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

void IntroState::OnCreate() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->window);

        std::shared_ptr<TextureManager>textureMgr(context->textureManager);
        textureMgr->RequireResource("Rose");

        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        sf::Vector2u windowSize = renderWindow->getSize();
        _introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

        std::shared_ptr<sf::Texture>introTexture(textureMgr->GetResource("Rose"));
        _introSprite.setTexture(*introTexture);
        _introSprite.setOrigin(windowSize.x / _introSprite.getLocalBounds().width,
                                windowSize.y / _introSprite.getLocalBounds().width);

        sf::FloatRect textRect = _text.getLocalBounds();
        std::shared_ptr<FontManager>fontMgr(context->fontManager);
        std::shared_ptr<sf::Font>mainFont(fontMgr->GetResource("Main"));
        _text.setFont(*mainFont);
        _text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        _text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
        _text.setString({ "Press SPACE to continue" });
        _text.setFillColor(sf::Color::Cyan);
        _text.setCharacterSize(40);

        std::shared_ptr<EventManager>evMgr(context->eventManager);
        auto lambdaContinue = [this](EventDetails& details) { this->Continue(details); };
        evMgr->AddCallback(StateType::Intro, "Intro_Continue", lambdaContinue);

        std::shared_ptr<SoundManager>soundMgr(context->soundManager);
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

        std::shared_ptr<EventManager>evMgr(context->eventManager);
        evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - ondestroy] " << e.what();
    }
}

void IntroState::Draw() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        renderWindow->clear(sf::Color(255,192,203 ,1));
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

void IntroState::Update(const sf::Time& time) {
    _introSprite.rotate(time.asSeconds());
}

void IntroState::Activate() {}
void IntroState::Deactivate() {}