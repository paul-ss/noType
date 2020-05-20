#include "gameState.hpp"

GameState::GameState(std::weak_ptr<StateManager> stateManager)
    : BaseState(stateManager) {}

void GameState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<EventManager> eMgr(context->eventManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        auto windowSize = renderWindow->getSize();
        auto filler = std::make_shared<Label>(context, sf::Vector2f(0, 0), "filler.json");
        sf::Vector2f fillerPosition(windowSize.x * 0.5f - filler->GetSize().x * 0.5,
                                    windowSize.y * 0.5f);
        filler->SetPosition(fillerPosition);
        //_elements.push_back(filler);
        _elements.emplace()

        _elements.push_back();

        //auto play = std::make_shared<Label>(context, sf::Vector2f(0, 0), "button.json");
        //auto playSize = play->GetSize();
        //sf::Vector2f playPosition((windowSize.x * 0.5f - playSize.x * 0.5),
        //                    (windowSize.y * 0.5f - playSize.y * 0.5) + ELEM_MARGIN_Y);
        //play->SetText("Play");
        //play->SetPosition(playPosition);
        //_elements.push_back(play);
        auto smartString = std::make_shared<SmartString>(context,
                sf::Vector2f((windowSize.x * 0.5), (windowSize.y * 0.5f)),
                "smartString.json",
                "Welcome to Twin Peaks.");
        auto smartStrSize = smartString->GetSize();
        sf::Vector2f smartStrPosition((windowSize.x * 0.5), (windowSize.y * 0.5f));
        smartString->SetPosition(smartStrPosition);
        _elements.push_back(smartString);

        auto lambdaQuit = [this](EventDetails& l_details) { this->MainMenu(l_details); };
        eMgr->AddCallback(StateType::Game, "Key_Escape", lambdaQuit);

        auto lambdaTextEntered = [this](EventDetails& l_details) { this->TextEntered(l_details); };
        eMgr->AddCallback(StateType::Game, "Text_Entered", lambdaTextEntered);
        eMgr->AddCallback(StateType::Game, "Text_Delete", lambdaTextEntered);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[game - oncreate] " << e.what();
        return;
    }
}

void GameState::TextEntered(EventDetails& l_details) {
    char input = l_details.textEntered;
    if (auto str = std::dynamic_pointer_cast<SmartString>(_elements[1])) {
        str->Validate(input);
    } else {
        //log
    }
}

void GameState::MainMenu(EventDetails& l_details) {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        stateMgr->SwitchTo(StateType::MainMenu);
    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[game - mainmenu] " << e.what();
        return;
    }
}

void GameState::OnDestroy() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [game - ondestroy]";
        return;
    }
    auto context = stateMgr->GetContext().lock();
    if (!context) {
        BOOST_LOG_TRIVIAL(error) << "Not valid shared context [game - ondestroy]";
        return;
    }
    auto eMgr = context->eventManager.lock();
    if (!eMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid event manager [game - ondestroy]";
        return;
    }
    eMgr->RemoveCallback(StateType::Game,"Key_Escape");
}

void GameState::Update(const sf::Time& time) {}
void GameState::Draw() {
    for (auto& element : _elements) {
        element->Draw();
    }
}
void GameState::Activate() {}
void GameState::Deactivate() {}
