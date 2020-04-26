#include "mainMenuState.hpp"
#include "stateManager.hpp"

MainMenuState::MainMenuState(StateManager* stateManager)
    : BaseState(stateManager) {}

MainMenuState::~MainMenuState() {}

void MainMenuState::OnCreate() {
    _font.loadFromFile("resources/media/fonts/arcade.ttf");
    _text.setFont(_font);
    _text.setString(sf::String("MAIN MENU"));
    _text.setCharacterSize(18);

    sf::FloatRect textRect = _text.getLocalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);

    _text.setPosition(400,100);

    _buttonSize = sf::Vector2f(300.0f,32.0f);
    _buttonPos = sf::Vector2f(400,200);
    _buttonPadding = 4;

    std::string str[3];
    str[0] = "PLAY";
    str[1] = "CREDITS";
    str[2] = "EXIT";

    for(int i = 0; i < 3; ++i) {
        sf::Vector2f buttonPosition(_buttonPos.x, _buttonPos.y + 
            (i * (_buttonSize.y + _buttonPadding)));
        _rects[i].setSize(_buttonSize);
        _rects[i].setFillColor(sf::Color::Cyan);

        _rects[i].setOrigin(_buttonSize.x / 2.0f, _buttonSize.y / 2.0f);
        _rects[i].setPosition(buttonPosition);

        _labels[i].setFont(_font);
        _labels[i].setString(sf::String(str[i]));
        _labels[i].setCharacterSize(12);

        sf::FloatRect rect = _labels[i].getLocalBounds();
        _labels[i].setOrigin(rect.left + rect.width / 2.0f,
            rect.top + rect.height / 2.0f);

        _labels[i].setPosition(buttonPosition);
    }

    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;
    auto lambdaClick =
            [this](EventDetails& details) {
                this->MouseClick(&details);
            };
    evMgr->AddCallback(StateType::MainMenu, "Mouse_Left", lambdaClick);
}

void MainMenuState::OnDestroy() {
    EventManager* evMgr = _stateMgr->GetContext()->_eventManager;
    evMgr->RemoveCallback(StateType::MainMenu, "Mouse_Left");
}

void MainMenuState::Activate() {
    if(_stateMgr->HasState(StateType::Game)
        && _labels[0].getString() == "PLAY")
    {
        _labels[0].setString(sf::String("RESUME"));
        sf::FloatRect rect = _labels[0].getLocalBounds();
        _labels[0].setOrigin(rect.left + rect.width / 2.0f,
        rect.top + rect.height / 2.0f);
    }
}

void MainMenuState::MouseClick(EventDetails* details) {
    sf::Vector2i mousePos = details->_mouse;

    float halfX = _buttonSize.x / 2.0f;
    float halfY = _buttonSize.y / 2.0f;
    for(int i = 0; i < 3; ++i) {
        if(mousePos.x >= _rects[i].getPosition().x - halfX &&
            mousePos.x <= _rects[i].getPosition().x + halfX &&
            mousePos.y >= _rects[i].getPosition().y - halfY &&
            mousePos.y <= _rects[i].getPosition().y + halfY)
        {
            if(i == 0){
                _stateMgr->SwitchTo(StateType::Game);
            } else if(i == 1){
                // Credits state.
            } else if(i == 2){
                _stateMgr->GetContext()->_window->Close();
            }
        }
    }
}

void MainMenuState::Draw() {
    sf::RenderWindow* window = _stateMgr->GetContext()->_window->GetRenderWindow();
    window->clear(sf::Color::Magenta);
    window->draw(_text);
    for(int i = 0; i < 3; ++i){
        window->draw(_rects[i]);
        window->draw(_labels[i]);
    }
}

void MainMenuState::Update(const sf::Time& time) {}
void MainMenuState::Deactivate() {}
