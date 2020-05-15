#include "mainMenuState.hpp"
#include "logger.hpp"

MainMenuState::MainMenuState(std::weak_ptr<StateManager> stateManager)
    : BaseState(stateManager) {}

void MainMenuState::OnCreate() {
    /*m_font.loadFromFile("assets/media/fonts/arcade.ttf");
    m_text.setFont(m_font);
    m_text.setString(sf::String("MAIN MENU"));
    m_text.setCharacterSize(18);

    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f,
    textRect.top + textRect.height / 2.0f);

    m_text.setPosition(400,100);

    m_buttonSize = sf::Vector2f(300.0f,32.0f);
    m_buttonPos = sf::Vector2f(400,200);
    m_buttonPadding = 4;

    std::string str[3];
    str[0] = "PLAY";
    str[1] = "CREDITS";
    str[2] = "EXIT";

    for(int i = 0; i < 3; ++i) {
        sf::Vector2f buttonPosition(m_buttonPos.x,m_buttonPos.y + 
            (i * (m_buttonSize.y + m_buttonPadding)));
        _rect->setSize(m_buttonSize);
        _rect->setFillColor(sf::Color::Red);

        _rect->setOrigin(m_buttonSize.x / 2.0f, m_buttonSize.y / 2.0f);
        _rect->setPosition(buttonPosition);

        _label->setFont(m_font);
        _label->setString(sf::String(t->);
        _label->setCharacterSize(12);

        sf::FloatRect rect = _label->getLocalBounds();
        _label->setOrigin(rect.left + rect.width / 2.0f,
            rect.top + rect.height / 2.0f);

        _label->setPosition(buttonPosition);
    }*/

    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<EventManager> eMgr(context->eventManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        sf::Vector2u windowSize = renderWindow->getSize();
        sf::Vector2f pos(windowSize.x / 2.0f, windowSize.y / 2.0f);
        auto quit = std::make_shared<Button>(context, pos, "button.json");
        quit->SetText("Quit");
        _elements.push_back(quit);

        auto lambdaQuit = [this](EventDetails& l_details) { this->Quit(l_details); };
        eMgr->AddCallback(StateType::MainMenu, "Key_Escape", lambdaQuit);

        auto lambdaClick = [this](EventDetails& l_details) { this->MouseClick(l_details); };
        eMgr->AddCallback(StateType::MainMenu, "Mouse_Left", lambdaClick);

        auto lambdaTextEntered = [this](EventDetails& l_details) { this->TextEntered(l_details); };
        eMgr->AddCallback(StateType::MainMenu, "Text_Entered", lambdaTextEntered);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - oncreate] " << e.what();
        return;
    }
}

void MainMenuState::TextEntered(EventDetails& l_details) {
    std::cout<< static_cast<char>(l_details.textEntered) << "\n";
}

void MainMenuState::OnDestroy() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<EventManager>eMgr(context->eventManager);

        eMgr->RemoveCallback(StateType::MainMenu, "Key_Escape");
        eMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
        eMgr->RemoveCallback(StateType::MainMenu, "Text_Entered");
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[mainMenu - ondestroy] " << e.what();
    }
}

void MainMenuState::Play(EventDetails& l_details) {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - play]";
        return;
    }
    stateMgr->SwitchTo(StateType::Game);
}

void MainMenuState::MouseClick(EventDetails& l_details) {
    sf::Vector2i mousePos = l_details.mouse;

    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<EventManager>eMgr(context->eventManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::Window>renderWindow(window->GetRenderWindow());

        for(auto& element : _elements) {
            float halfX = element->GetSize().x / 2.0f;
            float halfY = element->GetSize().y / 2.0f;
            if (mousePos.x >= element->GetPosition().x - halfX &&
                mousePos.x <= element->GetPosition().x + halfX &&
                mousePos.y >= element->GetPosition().y - halfY &&
                mousePos.y <= element->GetPosition().y + halfY) {

                window->Close();
            }
        }

    } catch (const std::bad_weak_ptr &e) {
        // log
    }

}

void MouseRelease(EventDetails& details) {}

void MainMenuState::Quit(EventDetails& details) {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Window>window(context->window);
        window->Close();

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[menu - quit] " << e.what();
    }
}

void MainMenuState::Activate() {}

void MainMenuState::Draw() {
    /*try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<EventManager>eMgr(context->eventManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        renderWindow->draw(m_text);
        for(int i = 0; i < 3; ++i) {
            renderWindow->draw(_rect->;
            renderWindow->draw(_label->;
        }
    } catch (const std::bad_weak_ptr &e) {
        //log
    }*/
    for (auto& element : _elements) {
        element->Draw();
    }

}

void MainMenuState::Update(const sf::Time& dT) {}
void MainMenuState::Deactivate() {}
