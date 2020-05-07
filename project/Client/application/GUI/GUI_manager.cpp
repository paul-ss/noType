#include <fstream>
#include <filesystem>

#include <boost/log/trivial.hpp>

#include "GUI_manager.hpp"
#include "GUI_textField.hpp"
#include "GUI_label.hpp"

GUI_Manager::GUI_Manager(EventManager* evMgr, SharedContext* shared)
    : _context(shared), _eventMgr(evMgr), _currentState(StateType(0)) {

    registerElement<GUI_Label>(GUI_ElementType::Label);
    registerElement<GUI_Textfield>(GUI_ElementType::Textfield);

    _elemTypes.emplace("Label", GUI_ElementType::Label);
    _elemTypes.emplace("Button", GUI_ElementType::Button);
    _elemTypes.emplace("TextField", GUI_ElementType::Textfield);
    _elemTypes.emplace("Interface", GUI_ElementType::Window);

    auto lambdaClick = [this](EventDetails& details) { this->HandleClick(&details); };
    _eventMgr->AddCallback(StateType(0), "Mouse_Left", lambdaClick);

    auto lambdaRelease = [this](EventDetails& details) { this->HandleRelease(&details); };
    _eventMgr->AddCallback(StateType(0), "Mouse_Left_Release", lambdaRelease);

    auto lambdaTextEntered = [this](EventDetails& details) { this->HandleTextEntered(&details); };
    _eventMgr->AddCallback(StateType(0), "Text_Entered", lambdaTextEntered);
}

GUI_Manager::~GUI_Manager() {
    _eventMgr->RemoveCallback(StateType(0), "Mouse_Left");
    _eventMgr->RemoveCallback(StateType(0), "Mouse_Left_Release");
    _eventMgr->RemoveCallback(StateType(0), "Text_Entered");

    for (auto &itr : _interfaces) {
        for (auto &itr2 : itr.second) {
            delete itr2.second;
        }
    }
}

GUI_Interface* GUI_Manager::GetInterface(const StateType& state,
    const std::string& name) {
    auto s = _interfaces.find(state);
    if (s == _interfaces.end()) {
        return nullptr;
    }

    auto i = s->second.find(name);
    return (i != s->second.end() ? i->second : nullptr);
}

bool GUI_Manager::RemoveInterface(const StateType& state,
    const std::string& name) {
    auto s = _interfaces.find(state);
    if (s == _interfaces.end()) {
        return false;
    }

    auto i = s->second.find(name);
    if (i == s->second.end()) {
        return false;
    }

    delete i->second;
    return s->second.erase(name);
}

void GUI_Manager::SetCurrentState(const StateType& state) {
    if (_currentState == state) {
        return;
    }

    HandleRelease(nullptr);
    _currentState = state;
}

SharedContext* GUI_Manager::GetContext() {
    return _context;
}

void GUI_Manager::DefocusAllInterfaces() {
    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
        return;
    }

    for (auto &itr : state->second) {
        itr.second->Defocus();
    }
}

void GUI_Manager::HandleClick(EventDetails* details) {
    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
        return;
    }

    sf::Vector2i mousePos = _eventMgr->GetMousePos(_context->_window->GetRenderWindow());
    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
        if (!itr->second->IsInside(sf::Vector2f(mousePos))) {
            continue;
        }

        if (!itr->second->IsActive()) {
            return;
        }

        itr->second->OnClick(sf::Vector2f(mousePos));
        itr->second->Focus();
        if (itr->second->IsBeingMoved()) {
            itr->second->BeginMoving();
        }
        return;
    }
}

void GUI_Manager::HandleRelease(EventDetails* details) {
    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
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

void GUI_Manager::HandleTextEntered(EventDetails* details) {
    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
        return;
    }

    for (auto &itr : state->second) {
        if (!itr.second->IsActive()) {
            continue;
        }

        if (!itr.second->IsFocused()) {
            continue;
        }

        itr.second->OnTextEntered(details->_textEntered);
        return;
    }
}

void GUI_Manager::AddEvent(GUI_Event event) {
    _events[_currentState].push_back(event);
}

bool GUI_Manager::PollEvent(GUI_Event& event) {
    if (_events[_currentState].empty()) {
        return false;
    }

    event = _events[_currentState].back();
    _events[_currentState].pop_back();
    return true;
}

void GUI_Manager::Update(float dT) {
    sf::Vector2i mousePos = _eventMgr->GetMousePos(_context->_window->GetRenderWindow());

    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
        return;
    }

    for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
        GUI_Interface* i = itr->second;
        if (!i->IsActive()) {
            continue;
        }

        i->Update(dT);
        if (i->IsBeingMoved()) {
            continue;
        }

        if (i->IsInside(sf::Vector2f(mousePos))) {
            if (i->GetState() == GUI_ElementState::Neutral) {
                i->OnHover(sf::Vector2f(mousePos));
            }
            return;

        } else if (i->GetState() == GUI_ElementState::Focused) {
            i->OnLeave();
        }
    }
}

void GUI_Manager::Render(sf::RenderWindow* window) {
    auto state = _interfaces.find(_currentState);
    if (state == _interfaces.end()) {
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

        i->Draw(window);
    }
}

GUI_Element* GUI_Manager::createElement(const GUI_ElementType& id, GUI_Interface* owner) {
    if (id == GUI_ElementType::Window) {
        return new GUI_Interface("", this);
    }

    auto f = _factory.find(id);
    return (f != _factory.end() ? f->second(owner) : nullptr);
}

bool GUI_Manager::AddInterface(const StateType& state,
    const std::string& name) {
    auto s = _interfaces.emplace(state, GUI_Interfaces()).first;

    GUI_Interface* temp = new GUI_Interface(name, this);
    if (s->second.emplace(name, temp).second) {
        return true;
    }

    delete temp;
    return false;
}

bool GUI_Manager::LoadInterface(const StateType& state,
    const std::string& interface, const std::string& l_name)
{
    std::ifstream file;
    //file.open(utils::GetWorkingDirectory() + "assets/media/GUI_Interfaces/" + interface);
    file.open(std::filesystem::absolute("assets/media/GUI_interfaces/" + interface));
    std::string InterfaceName;

    if (!file.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load: " << interface;
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
            if (!AddInterface(state, l_name)) {
                BOOST_LOG_TRIVIAL(error) << "Failed adding interface: " << l_name;
                return false;
            }
            GUI_Interface* i = GetInterface(state, l_name);
            keystream >> *i;
            if (!loadStyle(style, i)) {
                BOOST_LOG_TRIVIAL(error) << "Failed adding interface: " << style << "for interface " << l_name;
            }
            i->SetContentSize(i->GetSize());
        } else if (key == "Element") {
            if (InterfaceName == "") {
                BOOST_LOG_TRIVIAL(error) << "Error: 'Element' outside or before declaration of 'Interface'!";
                continue;
            }
            std::string type;
            std::string name;
            sf::Vector2f position;
            std::string style;
            keystream >> type >> name >> position.x >> position.y >> style;
            std::cout << name << "\n";
            GUI_ElementType eType = stringToType(type);
            if (eType == GUI_ElementType::None) {
                std::cout << "Unknown element('" << l_name << "') type: '" << type << "'" << std::endl;
                BOOST_LOG_TRIVIAL(error) << "Unknown element('" << l_name << "') type: '" << type << "'";
                continue;
            }

            GUI_Interface* i = GetInterface(state, l_name);
            if (!i) {
                continue;
            }
            if (!i->AddElement(eType, name)) {
                std::cout << "add element false\n";
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

bool GUI_Manager::loadStyle(const std::string& path, GUI_Element* element) {
    std::ifstream file;
    //file.open(utils::GetWorkingDirectory() + "assets/media/GUI_Styles/" + path);
    file.open(std::filesystem::absolute("assets/media/GUI_styles/" + path));

    std::string currentState;
    GUI_Style ParentStyle;
    GUI_Style TemporaryStyle;
    if (!file.is_open()) {
        std::cout << "! Failed to load: " << path << std::endl;
        BOOST_LOG_TRIVIAL(error) <<"! Failed to load: " << path;
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
        if (type == "") {
            continue;
        }
        if (type == "State") {
            if (currentState != "") {
                BOOST_LOG_TRIVIAL(error) <<"Error: 'State' keyword found inside another state!";
                continue;
            }
            keystream >> currentState;
        } else if (type == "/State") {
            if (currentState == "") {
                BOOST_LOG_TRIVIAL(error) <<"Error: '/State' keyword found prior to 'State'!";
                continue;
            }
            GUI_ElementState state = GUI_ElementState::Neutral;
            if (currentState == "Hover") { state = GUI_ElementState::Focused; }
            else if (currentState == "Clicked") { state = GUI_ElementState::Clicked; }

            if (state == GUI_ElementState::Neutral) {
                ParentStyle = TemporaryStyle;
                element->UpdateStyle(GUI_ElementState::Neutral, TemporaryStyle);
                element->UpdateStyle(GUI_ElementState::Focused, TemporaryStyle);
                element->UpdateStyle(GUI_ElementState::Clicked, TemporaryStyle);
            } else {
                element->UpdateStyle(state, TemporaryStyle);
            }
            TemporaryStyle = ParentStyle;
            currentState = "";
        } else {
            // Handling style information.
            if (currentState == "") {
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

GUI_ElementType GUI_Manager::stringToType(const std::string& string) {
    auto t = _elemTypes.find(string);
    return (t != _elemTypes.end() ? t->second : GUI_ElementType::None);
}
