#include "GUI_interface.hpp"
#include "GUI_manager.hpp"
#include "sharedContext.hpp"
#include "utils.hpp"

#define FIRST_INTRESTING_ASCII_CODE 32
#define LAST_INTRESTING_ASCII_CODE 126
#define ASCII_BACKSLASH 8

GUI_Interface::GUI_Interface(const std::string& name, std::weak_ptr<GUI_Manager> guiManager) :
        GUI_Element(name, GUI_ElementType::Window, this),
        _parent(nullptr),
        _guiManager(guiManager),
        _showTitleBar(false),
        _movable(false),
        _beingMoved(false),
        _focused(false),
        _scrollHorizontal(0),
        _scrollVertical(0),
        _contentRedraw(true),
        _controlRedraw(true) {

    _backdropTexture = new sf::RenderTexture();
    _contentTexture = new sf::RenderTexture();
    _controlTexture = new sf::RenderTexture();
}

GUI_Interface::~GUI_Interface() {
    if (_backdropTexture) { delete _backdropTexture; }
    if (_contentTexture) { delete _contentTexture; }
    if (_controlTexture) { delete _controlTexture; }
    for (auto &itr : _elements) {
        delete itr.second;
    }
}

void GUI_Interface::SetPosition(const sf::Vector2f& pos) {
    GUI_Element::SetPosition(pos);
    _backdrop.setPosition(pos);
    _content.setPosition(pos);
    _control.setPosition(pos);
    _titleBar.setPosition(_position.x, _position.y - _titleBar.getSize().y);
    _visual._text.setPosition(_titleBar.getPosition() + _style[_state]._textPadding);
}

bool GUI_Interface::AddElement(const GUI_ElementType& type,
    const std::string& name) {
    if (_elements.find(name) != _elements.end()) {
        return false;
    }

    GUI_Element* element = nullptr;
    element = _guiManager->createElement(type, this);
    if (!element) {
        return false;
    }
    element->SetName(name);
    element->SetOwner(this);
    _elements.emplace(name, element);
    _contentRedraw = true;
    _controlRedraw = true;
    return true;
}

GUI_Element* GUI_Interface::GetElement(const std::string& l_name) const {
    auto itr = _elements.find(l_name);
    return (itr != _elements.end() ? itr->second : nullptr);
}

bool GUI_Interface::RemoveElement(const std::string& l_name) {
    auto itr = _elements.find(l_name);
    if (itr == _elements.end()) {
        return false;
    }

    delete itr->second;
    _elements.erase(itr);
    _contentRedraw = true;
    _controlRedraw = true;
    AdjustContentSize();
    return true;
}

bool GUI_Interface::HasParent() const {
    return _parent != nullptr;
}

GUI_Manager* GUI_Interface::GetManager() const {
    return _guiManager;
}

bool GUI_Interface::IsInside(const sf::Vector2f& point) const {
    if (GUI_Element::IsInside(point)) {
        return true;
    }
    return _titleBar.getGlobalBounds().contains(point);
}

void GUI_Interface::Focus() {
    _focused = true;
}

void GUI_Interface::Defocus() {
    _focused = false;
}

bool GUI_Interface::IsFocused() {
    return _focused;
}

void GUI_Interface::ReadIn(const std::string& l_stream) {
    std::string movableState;
    std::string titleShow;
    std::string title;
    l_stream >> _elementPadding.x >> _elementPadding.y
            >> movableState >> titleShow;
    utils::ReadQuotedString(l_stream, title);
    _visual._text.setString(title);
    if (movableState == "Movable") {
        _movable = true;
    }
    if (titleShow == "Title") {
        _showTitleBar = true;
    }
}

void GUI_Interface::OnClick(const sf::Vector2f& l_mousePos) {
    DefocusTextfields();
    if (_titleBar.getGlobalBounds().contains(l_mousePos) &&
        _movable && _showTitleBar)
    {
        _beingMoved = true;
    } else {
        GUI_Event event;
        event._type = GUI_EventType::Click;
        event._interface = _name.c_str();
        event._element = "";
        event._clickCoords.x = l_mousePos.x;
        event._clickCoords.y = l_mousePos.y;
        _guiManager->AddEvent(event);
        for (auto &itr : _elements) {
            if (!itr.second->IsInside(l_mousePos)) {
                continue;
            }
            itr.second->OnClick(l_mousePos);
            event._element = itr.second->_name.c_str();
            _guiManager->AddEvent(event);
        }
        SetState(GUI_ElementState::Clicked);
    }
}

void GUI_Interface::OnRelease() {
    GUI_Event event;
    event._type = GUI_EventType::Release;
    event._interface = _name.c_str();
    event._element = "";
    _guiManager->AddEvent(event);
    for (auto &itr : _elements) {
        if (itr.second->GetState() != GUI_ElementState::Clicked) {
            continue;
        }
        itr.second->OnRelease();
        event._element = itr.second->_name.c_str();
        _guiManager->AddEvent(event);
    }
    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnHover(const sf::Vector2f& l_mousePos) {
    GUI_Event event;
    event._type = GUI_EventType::Hover;
    event._interface = _name;
    event._element = "";
    event._clickCoords.x = l_mousePos.x;
    event._clickCoords.y = l_mousePos.y;
    _guiManager->AddEvent(event);

    SetState(GUI_ElementState::Focused);
}

void GUI_Interface::OnLeave() {
    GUI_Event event;
    event._type = GUI_EventType::Leave;
    event._interface = _name;
    event._element = "";
    _guiManager->AddEvent(event);

    SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnTextEntered(const char& l_char) {
    for (auto &itr : _elements) {
        if (itr.second->GetType() != GUI_ElementType::Textfield) {
            continue;
        }
        if (itr.second->GetState() != GUI_ElementState::Clicked) {
            continue;
        }
        if (l_char == ASCII_BACKSLASH) {
            const auto& text = itr.second->GetText();
            itr.second->SetText(text.substr(0, text.length() -1));
            return;
        }
        if (l_char < FIRST_INTRESTING_ASCII_CODE || l_char > LAST_INTRESTING_ASCII_CODE) {
            return;
        }
        std::string text = itr.second->GetText();
        text.push_back(l_char);
        itr.second->SetText(text);
        return;
    }
}

const sf::Vector2f& GUI_Interface::GetPadding() const {
    return _elementPadding;
}

void GUI_Interface::SetPadding(const sf::Vector2f& l_padding) {
    _elementPadding = l_padding;
}

void GUI_Interface::Update(float l_dT) {
    sf::Vector2f mousePos = sf::Vector2f(
        _guiManager->GetContext()->_eventManager.lock()->GetMousePos(
        _guiManager->GetContext()->_window.lock()->GetRenderWindow()));

    if (_beingMoved && _moveMouseLast != mousePos) {
        sf::Vector2f difference = mousePos - _moveMouseLast;
        _moveMouseLast = mousePos;
        sf::Vector2f newPosition = _position + difference;
        SetPosition(newPosition);
    }

    for (auto &itr : _elements) {
        if (itr.second->NeedsRedraw()) {
            if (itr.second->IsControl()) { _controlRedraw = true; } 
            else { _contentRedraw = true; }
        }
        if (!itr.second->IsActive()) {
            continue;
        }
        itr.second->Update(l_dT);
        if (_beingMoved) {
            continue;
        }
        GUI_Event event;
        event._interface = _name.c_str();
        event._element = itr.second->_name.c_str();
        event._clickCoords.x = mousePos.x;
        event._clickCoords.y = mousePos.y;
        if (IsInside(mousePos) && itr.second->IsInside(mousePos) 
            && !_titleBar.getGlobalBounds().contains(mousePos))
        {
            if (itr.second->GetState() != GUI_ElementState::Neutral) {
                continue;
            }
            itr.second->OnHover(mousePos);
            event._type = GUI_EventType::Hover;
            _guiManager->AddEvent(event);
        } else if (itr.second->GetState() == GUI_ElementState::Focused) {
            itr.second->OnLeave();
            event._type = GUI_EventType::Leave;
            _guiManager->AddEvent(event);
        }
    }
}

void GUI_Interface::Draw(std::weak_ptr<sf::RenderWindow> l_target) {
    l_target.draw(_backdrop);
    l_target.draw(_content);
    l_target.draw(_control);

    if (!_showTitleBar) {
        return;
    }
    l_target.draw(_titleBar);
    l_target.draw(_visual._text);
}

bool GUI_Interface::IsBeingMoved() const {
    return _beingMoved;
}

bool GUI_Interface::IsMovable() const {
    return _movable;
}

void GUI_Interface::BeginMoving() {
    if (!_showTitleBar || !_movable) {
        return;
    }

    _beingMoved = true;
    SharedContext* context = _guiManager->GetContext();
    _moveMouseLast = sf::Vector2f(context->_eventManager->
            GetMousePos(context->_window->GetRenderWindow()));
}

void GUI_Interface::StopMoving() {
    _beingMoved = false;
}

sf::Vector2f GUI_Interface::GetGlobalPosition() const {
    sf::Vector2f pos = _position;
    GUI_Interface* i = _parent;
    while (i) {
        pos += i->GetPosition();
        i = i->_parent;
    }
    return pos;
}

void GUI_Interface::ApplyStyle() {
    GUI_Element::ApplyStyle(); // Call base method.
    _visual._backgroundSolid.setPosition(0.f,0.f);
    _visual._backgroundImage.setPosition(0.f,0.f);
    _titleBar.setSize(sf::Vector2f(_style[_state]._size.x, 16.f));
    _titleBar.setPosition(_position.x,_position.y - _titleBar.getSize().y);
    _titleBar.setFillColor(_style[_state]._elementColor);
    _visual._text.setPosition(_titleBar.getPosition() + _style[_state]._textPadding);
    _visual._glyph.setPosition(_titleBar.getPosition() + _style[_state]._glyphPadding);
}

void GUI_Interface::Redraw() {
    if (_backdropTexture->getSize().x != _style[_state]._size.x ||
        _backdropTexture->getSize().y != _style[_state]._size.y)
    {
        _backdropTexture->create(_style[_state]._size.x, _style[_state]._size.y);
    }
    _backdropTexture->clear(sf::Color(0, 0, 0, 0));
    ApplyStyle();
    _backdropTexture->draw(_visual._backgroundSolid);

    if (_style[_state]._backgroundImage != "") {
        _backdropTexture->draw(_visual._backgroundImage);
    }

    _backdropTexture->display();
    _backdrop.setTexture(_backdropTexture->getTexture());
    _backdrop.setTextureRect(sf::IntRect(0, 0, _style[_state]._size.x, _style[_state]._size.y));
    SetRedraw(false);
}

bool GUI_Interface::NeedsContentRedraw() const {
    return _contentRedraw;
}

void GUI_Interface::RedrawContent() {
    if (_contentTexture->getSize().x != _contentSize.x ||
        _contentTexture->getSize().y != _contentSize.y)
    {
        _contentTexture->create(_contentSize.x, _contentSize.y);
    }

    _contentTexture->clear(sf::Color(0, 0, 0, 0));

    for (auto &itr : _elements) {
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()) {
            continue;
        }
        element->ApplyStyle();
        element->Draw(_contentTexture);
        element->SetRedraw(false);
    }

    _contentTexture->display();
    _content.setTexture(_contentTexture->getTexture());

    _content.setTextureRect(sf::IntRect(_scrollHorizontal,
            _scrollVertical,
            _style[_state]._size.x,
            _style[_state]._size.y));
    _contentRedraw = false;
}

bool GUI_Interface::NeedsControlRedraw() const {
    return _controlRedraw;
}

void GUI_Interface::RedrawControls() {
    if (_controlTexture->getSize().x != _style[_state]._size.x ||
        _controlTexture->getSize().y != _style[_state]._size.y)
    {
        _controlTexture->create(_style[_state]._size.x, _style[_state]._size.y);
    }
    _controlTexture->clear(sf::Color(0, 0, 0, 0));

    for (auto &itr : _elements) {
        GUI_Element* element = itr.second;
        if (!element->IsActive() || !element->IsControl()) {
            continue;
        }
        element->ApplyStyle();
        element->Draw(_controlTexture);
        element->SetRedraw(false);
    }

    _controlTexture->display();
    _control.setTexture(_controlTexture->getTexture());
    _control.setTextureRect(sf::IntRect(0, 0, _style[_state]._size.x, _style[_state]._size.y));
    _controlRedraw = false;
}

void GUI_Interface::ToggleTitleBar() {
    _showTitleBar = !_showTitleBar;
}

void GUI_Interface::AdjustContentSize(std::shared_ptr<GUI_Element> reference) {
    if (reference) {
        sf::Vector2f bottomRight = reference->GetPosition() + reference->GetSize();
        if (bottomRight.x > _contentSize.x) {
            _contentSize.x = bottomRight.x;
            _controlRedraw = true;
        }
        if (bottomRight.y > _contentSize.y) {
            _contentSize.y = bottomRight.y;
            _controlRedraw = true;
        }
        return;
    }

    sf::Vector2f farthest = GetSize();

    for (auto &itr : _elements) {
        GUI_Element* element = itr.second;
        if (!element->IsActive() || element->IsControl()) {
            continue;
        }
        sf::Vector2f bottomRight = element->GetPosition() + element->GetSize();
        if (bottomRight.x > farthest.x) {
            farthest.x = bottomRight.x;
            _controlRedraw = true;
        }
        if (bottomRight.y > farthest.y) {
            farthest.y = bottomRight.y;
            _controlRedraw = true;
        }
    }
    SetContentSize(farthest);
}

void GUI_Interface::SetContentSize(const sf::Vector2f& l_vec) {
    _contentSize = l_vec;
}

void GUI_Interface::UpdateScrollHorizontal(unsigned int l_percent) {
    if (l_percent > 100) {
        return;
    }
    _scrollHorizontal = ((_contentSize.x - GetSize().x) / 100) * l_percent;
    sf::IntRect rect = _content.getTextureRect();
    _content.setTextureRect(sf::IntRect(_scrollHorizontal, _scrollVertical, rect.width, rect.height));
}

void GUI_Interface::UpdateScrollVertical(unsigned int l_percent) {
    if (l_percent > 100) {
        return;
    }
    _scrollVertical = ((_contentSize.y - GetSize().y) / 100) * l_percent;
    sf::IntRect rect = _content.getTextureRect();
    _content.setTextureRect(sf::IntRect(_scrollHorizontal, _scrollVertical, rect.width, rect.height));
}

const sf::Vector2f& GUI_Interface::GetContentSize() const {
    return _contentSize;
}

void GUI_Interface::DefocusTextfields() {
    GUI_Event event;
    event._type = GUI_EventType::Release;
    event._interface = _name.c_str();
    event._element = "";
    for (auto &itr : _elements) {
        if (itr.second->GetType() != GUI_ElementType::Textfield) {
            continue;
        }
        itr.second->SetState(GUI_ElementState::Neutral);
        event._element = itr.second->_name.c_str();
        _guiManager->AddEvent(event);
    }
}
