#include "GUI_element.hpp"
#include "GUI_interface.hpp"
#include "GUI_manager.hpp"
#include "sharedContext.hpp"

GUI_Element::GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner)
    : _name(name), _type(type), _state(GUI_ElementState::Neutral), _owner(owner),
    _needsRedraw(false), _active(true), _isControl(false) {}

GUI_Element::~GUI_Element() {
    releaseResources();
}

void GUI_Element::releaseResources() {
    for (auto &itr : _style) {
        releaseTexture(itr.second._backgroundImage);
        releaseTexture(itr.second._glyph);
        releaseFont(itr.second._textFont);
    }
}

void GUI_Element::UpdateStyle(const GUI_ElementState& state,
    const GUI_Style& style) {
    // Resource management.
    if (style._backgroundImage != _style[state]._backgroundImage) {
        releaseTexture(_style[state]._backgroundImage);
        requireTexture(style._backgroundImage);
    }

    if (style._glyph != _style[state]._glyph){
        releaseTexture(_style[state]._glyph);
        requireTexture(style._glyph);
    }

    if (style._textFont != _style[state]._textFont){
        releaseFont(_style[state]._textFont);
        requireFont(style._textFont);
    }
    // Style application.
    _style[state] = style;
    if (state == _state) {
        SetRedraw(true);
        ApplyStyle();
    }
}

void GUI_Element::ApplyStyle() {
    applyTextStyle();
    applyBgStyle();
    applyGlyphStyle();
    if (_owner != this && !IsControl()) {
        _owner->AdjustContentSize(this);
    }
}

void GUI_Element::applyTextStyle() {
    FontManager* fonts = _owner->GetManager()->GetContext()->_fontManager;
    const GUI_Style& CurrentStyle = _style[_state];
    if (CurrentStyle._textFont != ""){
        _visual._text.setFont(*fonts->GetResource(CurrentStyle._textFont));
        _visual._text.setFillColor(CurrentStyle._textColor);
        _visual._text.setCharacterSize(CurrentStyle._textSize);
        if (CurrentStyle._textCenterOrigin){
            sf::FloatRect rect = _visual._text.getLocalBounds();
            _visual._text.setOrigin(rect.left + rect.width / 2.0f,
                rect.top + rect.height / 2.0f);
        } else {
            _visual._text.setOrigin(0.f, 0.f);
        }
    }
    _visual._text.setPosition(_position + CurrentStyle._textPadding);
}
void GUI_Element::applyBgStyle() {
    TextureManager* textures = _owner->GetManager()->GetContext()->_textureManager;
    const GUI_Style& CurrentStyle = _style[_state];
    if (CurrentStyle._backgroundImage != "") {
        _visual._backgroundImage.setTexture(*textures->GetResource(CurrentStyle._backgroundImage));
        _visual._backgroundImage.setColor(CurrentStyle._backgroundImageColor);
    }
    _visual._backgroundImage.setPosition(_position);
    _visual._backgroundSolid.setSize(sf::Vector2f(CurrentStyle._size));
    _visual._backgroundSolid.setFillColor(CurrentStyle._backgroundColor);
    _visual._backgroundSolid.setPosition(_position);
}

void GUI_Element::applyGlyphStyle() {
    const GUI_Style& CurrentStyle = _style[_state];
    TextureManager* textures = _owner->GetManager()->GetContext()->_textureManager;
    if (CurrentStyle._glyph != ""){
        _visual._glyph.setTexture(*textures->GetResource(CurrentStyle._glyph));
    }
    _visual._glyph.setPosition(_position + CurrentStyle._glyphPadding);
}

GUI_ElementType GUI_Element::GetType() {
    return _type;
}

void GUI_Element::SetState(const GUI_ElementState& state) {
    if (_state == state) {
        return;
    }
    _state = state;
    SetRedraw(true);
}

const std::string& GUI_Element::GetName() const {
    return _name;
}

void GUI_Element::SetName(const std::string& name) {
    _name = name;
}

const sf::Vector2f& GUI_Element::GetPosition() const {
    return _position;
}

void GUI_Element::SetPosition(const sf::Vector2f& pos) {
    _position = pos;
    if (_owner == nullptr || _owner == this) {
        return;
    }
    const auto& padding = _owner->GetPadding();
    if (_position.x < padding.x) {
        _position.x = padding.x;
    }
    if (_position.y < padding.y) {
        _position.y = padding.y;
    }
}

const sf::Vector2f& GUI_Element::GetSize() const {
    return _style.at(_state)._size;
}

GUI_ElementState GUI_Element::GetState() const {
    return _state;
}

void GUI_Element::SetRedraw(const bool& redraw) {
    _needsRedraw = redraw;
}

bool GUI_Element::NeedsRedraw() const {
    return _needsRedraw;
}

void GUI_Element::SetOwner(GUI_Interface* owner) {
    _owner = owner;
}

GUI_Interface* GUI_Element::GetOwner() const {
    return _owner;
}

bool GUI_Element::HasOwner() const {
    return _owner != nullptr;
}

bool GUI_Element::IsActive() const {
    return _active;
}

void GUI_Element::SetActive(const bool active) {
    if (active != _active) {
        _active = active; SetRedraw(true);
    }
}

bool GUI_Element::IsControl() const {
    return _isControl;
}

std::string GUI_Element::GetText() const {
    return _visual._text.getString();
}

void GUI_Element::SetText(const std::string& text) {
    _visual._text.setString(text);
    SetRedraw(true);
}

bool GUI_Element::IsInside(const sf::Vector2f& point) const {
    sf::Vector2f position = GetGlobalPosition();
    return(point.x >= position.x &&
        point.y >= position.y &&
        point.x <= (position.x + _style.at(_state)._size.x) &&
        point.y <= (position.y + _style.at(_state)._size.y));
}

sf::Vector2f GUI_Element::GetGlobalPosition() const{
    sf::Vector2f position = GetPosition();
    if (_owner == nullptr || _owner == this) {
        return position;
    }

    position += _owner->GetGlobalPosition();
    if (IsControl()) {
        return position;
    }

    position.x -= _owner->_scrollHorizontal;
    position.y -= _owner->_scrollVertical;
    return position;
}

void GUI_Element::requireTexture(const std::string& name){
    if (name == "") {
        return;
    }
    _owner->GetManager()->GetContext()->
            _textureManager->RequireResource(name);
}

void GUI_Element::requireFont(const std::string& name){
    if (name == "") {
        return;
    }
    _owner->GetManager()->GetContext()->
            _fontManager->RequireResource(name);
}

void GUI_Element::releaseTexture(const std::string& name){
    if (name == "") {
        return;
    }
    _owner->GetManager()->GetContext()->
        _textureManager->ReleaseResource(name);
}

void GUI_Element::releaseFont(const std::string& name){
    if (name == "") {
        return;
    }
    _owner->GetManager()->GetContext()->
        _fontManager->ReleaseResource(name);
}
