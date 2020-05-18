#include "mainMenuState.hpp"
#include "logger.hpp"

#define ELEM_MARGIN_X 80
#define ELEM_MARGIN_Y 100

MainMenuState::MainMenuState(std::weak_ptr<StateManager> stateManager)
    : BaseState(stateManager) {}

void MainMenuState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<EventManager> eMgr(context->eventManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        auto windowSize = renderWindow->getSize();
        auto filler = std::make_shared<Label>(context, sf::Vector2f(0, 0), "filler.json");
        _elements.push_back(filler);

        auto play = std::make_shared<Label>(context, sf::Vector2f(0, 0), "button.json");
        auto playSize = play->GetSize();
        sf::Vector2f playPosition((windowSize.x * 0.5f - playSize.x * 0.5),
                            (windowSize.y * 0.5f - playSize.y * 0.5) + ELEM_MARGIN_Y);
        play->SetText("Play");
        play->SetPosition(playPosition);
        _elements.push_back(play);

        auto quit = std::make_shared<Label>(context, sf::Vector2f(0, 0), "button.json");
        auto quitSize = play->GetSize();
        sf::Vector2f quitPosition((windowSize.x * 0.5f - playSize.x * 0.5),
                            (windowSize.y * 0.5f - playSize.y * 0.5) - ELEM_MARGIN_Y);
        quit->SetText("Quit");
        quit->SetPosition(quitPosition);
        _elements.push_back(quit);

        auto lambdaQuit = [this](EventDetails& l_details) { this->MouseClick(l_details); };
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
    //std::cout<< static_cast<char>(l_details.textEntered) << "\n";
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

        //for (size_t i = 0; i < _elements.size(); ++i) {
        //    float halfX = _elements[i]->GetSize().x / 2.0f;
        //    float halfY = _elements[i]->GetSize().y / 2.0f;
        //    if (mousePos.x >= _elements[i]->GetPosition().x - halfX &&
        //        mousePos.x <= _elements[i]->GetPosition().x + halfX &&
        //        mousePos.y >= _elements[i]->GetPosition().y - halfY &&
        //        mousePos.y <= _elements[i]->GetPosition().y + halfY) {
        //        if(i == 1) {
                    stateMgr->SwitchTo(StateType::Game);
        //        } else if(i == 2) {
        //            window->Close();
        //        }
        //    }
        //}

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
    for (auto& element : _elements) {
        element->Draw();
    }
}

void MainMenuState::Update(const sf::Time& dT) {}
void MainMenuState::Deactivate() {}
