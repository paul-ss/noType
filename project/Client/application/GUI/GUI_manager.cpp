#include <fstream>
#include <filesystem>

#include "GUI_manager.hpp"
#include "GUI_textField.hpp"
#include "GUI_label.hpp"
#include "logger.hpp"

#define INTERFACE_FILE_PATH "assets/media/GUI_guiContainer/"
#define STYLE_FILE_PATH "assets/media/GUI_styles/"

GUI_Manager::GUI_Manager(std::weak_ptr<EventManager> l_eMgr, std::weak_ptr<SharedContext> l_shared) :
        _context(l_shared), _eventMgr(l_eMgr), _currentState(StateType(0)) {

    registerElement<GUI_Label>(GUI_ElementType::Label);
    registerElement<GUI_TextField>(GUI_ElementType::TextField);

    _elemTypes.emplace("Label", GUI_ElementType::Label);
    _elemTypes.emplace("Button", GUI_ElementType::Button);
    _elemTypes.emplace("TextField", GUI_ElementType::TextField);
    _elemTypes.emplace("Interface", GUI_ElementType::Window);

    try {
        std::shared_ptr<EventManager> eMgr(_eventMgr);
        auto lambdaClick = [this](EventDetails& l_details) {
            this->HandleClick(l_details);
        };
        eMgr->AddCallback(StateType(0), "Mouse_Left", lambdaClick);

        auto lambdaRelease = [this](EventDetails& l_details) {
            this->HandleRelease(l_details);
        };
        eMgr->AddCallback(StateType(0), "Mouse_Left_Release", lambdaRelease);

        auto lambdaTextEntered = [this](EventDetails& l_details) {
            this->HandleTextEntered(l_details);
        };
        eMgr->AddCallback(StateType(0), "Text_Entered", lambdaTextEntered);
    } catch (std::bad_weak_ptr& e) {
        //log
    }
}

GUI_Manager::~GUI_Manager() {
    try {
    std::shared_ptr<EventManager> eMgr(_eventMgr);
    eMgr->RemoveCallback(StateType(0), "Mouse_Left");
    eMgr->RemoveCallback(StateType(0), "Mouse_Left_Release");
    eMgr->RemoveCallback(StateType(0), "Text_Entered");
    } catch (std::bad_weak_ptr& e) {
        // log
    }
}

std::weak_ptr<GUI_Interface> GUI_Manager::GetInterface(const StateType& l_state,
    const std::string& l_name) {
    auto stateItr = _guiContainer.find(l_state);
    if (stateItr == _guiContainer.end()) {
        return nullptr;
    }

    auto interfaceItr = stateItr->second.find(l_name);
    return (interfaceItr != stateItr->second.end() ? interfaceItr->second.first :
            std::weak_ptr<GUI_Interface>());
}

bool GUI_Manager::RemoveInterface(const StateType& l_state,
    const std::string& l_name) {
    auto stateItr = _guiContainer.find(l_state);
    if (stateItr == _guiContainer.end()) {
        return false;
    }

    auto interfaceItr = s->second.find(l_name);
    if (interfaceItr == s->second.end()) {
        return false;
    }

    return stateItr->second.erase(l_name);
}

void GUI_Manager::SetCurrentState(const StateType& l_state) {
    if (_currentState == l_state) {
        return;
    }
    HandleRelease(nullptr);
    _currentState = l_state;
}

std::weak_ptr<SharedContext> GUI_Manager::GetContext() {
    return _context;
}

void GUI_Manager::DefocusAllInterfaces() {
    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    for (auto &itr : state->second) {
        itr.second->Defocus();
    }
}

void GUI_Manager::HandleClick(EventDetails& l_details) {
    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    sf::Vector2i mousePos = _eventMgr->GetMousePos(_context->_wind->GetRenderWindow());
    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
        if (!itr->second->IsInside(sf::Vector2f(mousePos))) {
            continue;
        }
        if (!itr->second->IsActive()) {
            return;
        }
        itr->second->OnClick(sf::Vector2f(mousePos));
        itr->second->Focus();
        if (itr->second->IsBeingMoved()) { itr->second->BeginMoving(); }
        return;
    }
}

void GUI_Manager::HandleRelease(EventDetails& l_details) {
    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    for (auto &itr : state->second) {
        GUI_Interface* i = itr.second;
        if (!i->IsActive()) {
            continue;
        }
        if (i->GetState() == GUI_ElementState::Clicked) {
            i->OnRelease();
        }
        if (i->IsBeingMoved()) {
            i->StopMoving();
        }
    }
}

void GUI_Manager::HandleTextEntered(EventDetails& l_details) {
    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    for (auto &itr : state->second) {
        if (!itr.second->IsActive()) {
            continue;
        }
        if (!itr.second->IsFocused()) {
            continue;
        }
        itr.second->OnTextEntered(l_details._textEntered);
        return;
    }
}

void GUI_Manager::AddEvent(GUI_Event l_event) {
    _events[_currentState].push_back(l_event);
}

bool GUI_Manager::PollEvent(GUI_Event& l_event) {
    if (_events[_currentState].empty()) {
        return false;
    }
    l_event = _events[_currentState].back();
    _events[_currentState].pop_back();
    return true;
}
void GUI_Manager::Update(float l_dT) {
    sf::Vector2i mousePos = _eventMgr->GetMousePos(_context->_wind->GetRenderWindow());

    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
        GUI_Interface* i = itr->second;
        if (!i->IsActive()) {
            continue;
        }
        i->Update(l_dT);
        if (i->IsBeingMoved()) {
            continue;
        }
        if (i->IsInside(sf::Vector2f(mousePos)))
        {
            if (i->GetState() == GUI_ElementState::Neutral) {
                i->OnHover(sf::Vector2f(mousePos));
            }
            return;
        } else if (i->GetState() == GUI_ElementState::Focused) {
            i->OnLeave();
        }
    }
}

void GUI_Manager::Render(std::weak_ptr<sf::RenderWindow> l_window) {
    auto state = _guiContainer.find(_currentState);
    if (state == _guiContainer.end()) {
        return;
    }
    for (auto &itr : state->second) {
        GUI_Interface* i = itr.second;
        if (!i->IsActive()) {
            continue;
        }
        if (i->NeedsRedraw()) {
            i->Redraw();
        }
        if (i->NeedsContentRedraw()) {
            i->RedrawContent();
        }
        if (i->NeedsControlRedraw()) {
            i->RedrawControls();
        }
        i->Draw(l_window);
    }
}

std::shared_ptr<GUI_Element> GUI_Manager::createElement(const GUI_ElementType& l_id, std::string& l_name) {
    if (l_id == GUI_ElementType::Window) {
        return std::make_shared<GUI_Interface>(std::string());
    }

    auto f = _factory.find(l_id);
    return (f != _factory.end() ? f->second(l_name) : nullptr);
}

bool GUI_Manager::AddInterface(const StateType& l_state,
    const std::string& l_name) {
    auto s = _guiContainer.emplace(l_state, GUI_guiContainer()).first;
    if (!s->second.emplace(l_name, GUI_Frame()).second) {
        //log
        return false;
    }
    return true;
}

std::weak_ptr<GUI_Interface> GUI_Manager::GetFrame( const StateType& l_type, cosnt std::string& l_name) {
    auto interface = _guiContainer.find(l_type);
    auto frame = interface.find(l_name);
    retrurn (frame ? frame.first() : nullptr);
}

bool GUI_Manager::LoadInterface(const StateType& l_state,
    const std::string& l_interface, const std::string& l_name) {

    std::ifstream file;
    file.open(std::filesystem::absolute(INTERFACE_FILE_PATH + l_interface));
    std::string InterfaceName;

    if (!file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load: " << l_interface;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '|') {
            continue;
        }
        std::stringstream keystream(line);
        std::string key;
        keystream >> key;
        if (key == "Interface") {
            std::string style;
            keystream >> InterfaceName >> style;
            if (!AddInterface(l_state, l_name)) {
                BOOST_LOG_TRIVIAL(error) << "Failed adding interface: " << l_name;
                return false;
            }
            GUI_Interface* i = GetInterface(l_state, l_name);
            keystream >> *i;
            if (!loadStyle(style, i)) {
                BOOST_LOG_TRIVIAL(error) << "Failed adding interface: " << style << "for interface " << l_name;
            }
            i->SetContentSize(i->GetSize());
        } else if (key == "Element") {
            if (InterfaceName.empty()) {
                BOOST_LOG_TRIVIAL(error) << "Error: 'Element' outside or before declaration of 'Interface'!";
                continue;
            }
            std::string type;
            std::string name;
            sf::Vector2f position;
            std::string style;
            keystream >> type >> name >> position.x >> position.y >> style;
            GUI_ElementType eType = stringToType(type);
            if (eType == GUI_ElementType::None) {
                std::cout << "Unknown element('" << l_name << "') type: '" << type << "'" << std::endl;
                BOOST_LOG_TRIVIAL(error) << "Unknown element('" << l_name << "') type: '" << type << "'";
                continue;
            }

            GUI_Interface* i = GetInterface(l_state, l_name);
            if (!i) {
                continue;
            }
            if (!i->AddElement(eType, name)) {
                continue;
            }
            GUI_Element* e = i->GetElement(name);
            keystream >> *e;
            e->SetPosition(position);
            if (!loadStyle(style, e)) {
                BOOST_LOG_TRIVIAL(error) <<"Failed loading style file: " << style << " for element " << name;
                continue;
            }
        }
    }
    file.close();
    return true;
}

bool GUI_Manager::loadStyle(const std::string& l_file, std::weak_ptr<GUI_Element> l_element) {
    std::ifstream file;
    file.open(std::filesystem::absolute(STYLE_FILE_PATH + l_file));

    std::string currentState;
    GUI_Style ParentStyle;
    GUI_Style TemporaryStyle;
    if (!file.is_open()) {
        std::cout << "! Failed to load: " << l_file << std::endl;
        BOOST_LOG_TRIVIAL(error) <<"! Failed to load: " << l_file;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == '|') {
            continue;
        }
        std::stringstream keystream(line);
        std::string type;
        keystream >> type;
        if (type.empty()) {
            continue;
        }
        if (type == "State") {
            if (!currentState.empty()) {
                BOOST_LOG_TRIVIAL(error) <<"Error: 'State' keyword found inside another state!";
                continue;
            }
            keystream >> currentState;
        } else if (type == "/State") {
            if (currentState.empty()) {
                BOOST_LOG_TRIVIAL(error) <<"Error: '/State' keyword found prior to 'State'!";
                continue;
            }
            GUI_ElementState state = GUI_ElementState::Neutral;
            if (currentState == "Hover") {
                state = GUI_ElementState::Focused;
            }
            else if (currentState == "Clicked") {
                state = GUI_ElementState::Clicked;
            }

            if (state == GUI_ElementState::Neutral) {
                ParentStyle = TemporaryStyle;
                l_element->UpdateStyle(GUI_ElementState::Neutral, TemporaryStyle);
                l_element->UpdateStyle(GUI_ElementState::Focused, TemporaryStyle);
                l_element->UpdateStyle(GUI_ElementState::Clicked, TemporaryStyle);
            } else {
                l_element->UpdateStyle(state, TemporaryStyle);
            }
            TemporaryStyle = ParentStyle;
            currentState = "";
        } else {
            // Handling style information.
            if (currentState.empty()) {
                BOOST_LOG_TRIVIAL(error) << "Error: '" << type << "' keyword found outside of a state!";
                continue;
            }
            if (type == "Size") {
                keystream >> TemporaryStyle._size.x >> TemporaryStyle._size.y;
            } else if (type == "BgColor") {
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle._backgroundColor = sf::Color(r,g,b,a);
            } else if (type == "BgImage") {
                keystream >> TemporaryStyle._backgroundImage;
            } else if (type == "BgImageColor") {
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle._backgroundImageColor = sf::Color(r, g, b, a);
            } else if (type == "TextColor") {
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle._textColor = sf::Color(r, g, b, a);
            } else if (type == "TextSize") {
                keystream >> TemporaryStyle._textSize;
            } else if (type == "TextOriginCenter") {
                TemporaryStyle._textCenterOrigin = true;
            } else if (type == "Font") {
                keystream >> TemporaryStyle._textFont;
            } else if (type == "TextPadding") {
                keystream >> TemporaryStyle._textPadding.x >> TemporaryStyle._textPadding.y;
            } else if (type == "ElementColor") {
                int r, g, b, a = 0;
                keystream >> r >> g >> b >> a;
                TemporaryStyle._elementColor = sf::Color(r, g, b, a);
            } else if (type == "Glyph") {
                keystream >> TemporaryStyle._glyph;
            } else if (type == "GlyphPadding") {
                keystream >> TemporaryStyle._glyphPadding.x >> TemporaryStyle._glyphPadding.y;
            } else {
                BOOST_LOG_TRIVIAL(error) <<"Error: style tag '" << type << "' is unknown!";
            }
        }
    }

    file.close();
    return true;
}

GUI_ElementType GUI_Manager::stringToType(const std::string& l_string) {
    auto t = _elemTypes.find(l_string);
    return (t != _elemTypes.end() ? t->second : GUI_ElementType::None);
}
