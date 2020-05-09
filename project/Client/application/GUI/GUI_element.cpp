#include "GUI_element.hpp"
#include "GUI_interface.hpp"
#include "GUI_manager.hpp"
#include "sharedContext.hpp"

GUI_Element::GUI_Element(const std::string& l_name, const GUI_ElementType& l_type,
                std::shared_ptr<GUI_Interface> l_owner) :
        _name(l_name),
        _type(l_type),
        _state(GUI_ElementState::Neutral),
        _owner(l_owner),
        _needsRedraw(false),
        _active(true),
        _isControl(false) {}

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

void GUI_Element::UpdateStyle(const GUI_ElementState& l_state,
    const GUI_Style& l_style)
{
    if (l_style._backgroundImage != _style[l_state]._backgroundImage) {
        releaseTexture(_style[l_state]._backgroundImage);
        requireTexture(l_style._backgroundImage);
    }

    if (l_style._glyph != _style[l_state]._glyph) {
        releaseTexture(_style[l_state]._glyph);
        requireTexture(l_style._glyph);
    }

    if (l_style._textFont != _style[l_state]._textFont) {
        releaseFont(_style[l_state]._textFont);
        requireFont(l_style._textFont);
    }
    _style[l_state] = l_style;
    if (l_state == _state) { SetRedraw(true); ApplyStyle(); }
}

void GUI_Element::ApplyStyle() {
    applyTextStyle();
    applyBgStyle();
    applyGlyphStyle();
    if (_owner.get() != this && !IsControl()) {
        _owner->AdjustContentSize(this);
    }
}

void GUI_Element::applyTextStyle() {
    try {
        std::shared_ptr<GUI_Interface> interface(_owner);
        std::shared_ptr<GUI_Manager> guiManager(interface->GetManager());
        std::shared_ptr<SharedContext> context(guiManager->GetContext());
        std::shared_ptr<FontManager> fontMgr(context->_fontManager);

        const GUI_Style& CurrentStyle = _style[_state];
        if (!CurrentStyle._textFont.empty()) {
            std::shared_ptr<sf::Font> font(fontMgr->GetResource(CurrentStyle._textFont));
            _visual._text.setFont(*font);
            _visual._text.setFillColor(CurrentStyle._textColor);
            _visual._text.setCharacterSize(CurrentStyle._textSize);
            if (CurrentStyle._textCenterOrigin) {
                sf::FloatRect rect = _visual._text.getLocalBounds();
                _visual._text.setOrigin(rect.left + rect.width / 2.0f,
                    rect.top + rect.height / 2.0f);
            } else {
                _visual._text.setOrigin(0.f, 0.f);
            }
        }
        _visual._text.setPosition(_position + CurrentStyle._textPadding);
    } catch(const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[gui_element - applyTextStyle] " << e.what();
    }
}

void GUI_Element::applyBgStyle() {
    try {
        std::shared_ptr<GUI_Interface> interface(_owner);
        std::shared_ptr<GUI_Manager> guiManager(interface->GetManager());
        std::shared_ptr<SharedContext> context(guiManager->GetContext());
        std::shared_ptr<TextureManager> textureMgr(context->_textureManager);

        const GUI_Style& CurrentStyle = _style[_state];
        if (!CurrentStyle._backgroundImage.empty()) {
            std::shared_ptr<sf::Texture> texture(textureMgr->GetResource(CurrentStyle._backgroundImage));
            _visual._backgroundImage.setTexture(*texture);
            _visual._backgroundImage.setColor(CurrentStyle._backgroundImageColor);
        }
        _visual._backgroundImage.setPosition(_position);
        _visual._backgroundSolid.setSize(sf::Vector2f(CurrentStyle._size));
        _visual._backgroundSolid.setFillColor(CurrentStyle._backgroundColor);
        _visual._backgroundSolid.setPosition(_position);
    } catch(std::bad_weak_ptr& e) {
        //log
    }
}

void GUI_Element::applyGlyphStyle() {
        try {
        std::shared_ptr<GUI_Interface> interface(_owner);
        std::shared_ptr<GUI_Manager> guiManager(interface->GetManager());
        std::shared_ptr<SharedContext> context(guiManager->GetContext());
        std::shared_ptr<TextureManager> textureMgr(context->_textureManager);

        const GUI_Style& CurrentStyle = _style[_state];
        if (!CurrentStyle._glyph.empty()) {
            std::shared_ptr<sf::Texture> texture(textureMgr->GetResource(CurrentStyle._glyph));
            _visual._glyph.setTexture(*texture);
        }
        _visual._glyph.setPosition(_position + CurrentStyle._glyphPadding);
    } catch (std::bad_weak_ptr& e) {
        // log
    }
}

GUI_ElementType GUI_Element::GetType() {
    return _type;
}

void GUI_Element::SetState(const GUI_ElementState& l_state) {
    if (_state == l_state) {
        return;
    }
    _state = l_state;
    SetRedraw(true);
}

const std::string& GUI_Element::GetName() const {
    return _name;
}

void GUI_Element::SetName(const std::string& l_name) {
    _name = l_name;
}

const sf::Vector2f& GUI_Element::GetPosition() const {
    return _position;
}

void GUI_Element::SetPosition(const sf::Vector2f& l_pos) {
    _position = l_pos;
    std::shared_ptr<GUI_Interface> interface(_owner);
    if (interface == nullptr || interface.get() == this) {
        return;
    }
    const auto& padding = interface->GetPadding();
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

void GUI_Element::SetRedraw(const bool& l_redraw) {
    _needsRedraw = l_redraw;
}

bool GUI_Element::NeedsRedraw() const {
    return _needsRedraw;
}

void GUI_Element::SetOwner(std::shared_ptr<GUI_Interface> l_owner) {
    _owner = l_owner;
}

std::weak_ptr<GUI_Interface> GUI_Element::GetOwner() const {
    return _owner;
}

bool GUI_Element::HasOwner() const {
    std::shared_ptr<GUI_Interface> interface(_owner);
    return interface != nullptr;
}

bool GUI_Element::IsActive() const {
    return _active;
}

void GUI_Element::SetActive(const bool& l_active) {
    if (l_active != _active) {
        _active = l_active;
        SetRedraw(true); 
    }
}

bool GUI_Element::IsControl() const {
    return _isControl;
}

std::string GUI_Element::GetText() const {
    return _visual._text.getString();
}

void GUI_Element::SetText(const std::string& l_text) {
    _visual._text.setString(l_text);
    SetRedraw(true);
}

bool GUI_Element::IsInside(const sf::Vector2f& l_point) const {
    sf::Vector2f position = GetGlobalPosition();
    return(l_point.x >= position.x &&
        l_point.y >= position.y &&
        l_point.x <= position.x + _style.at(_state)._size.x &&
        l_point.y <= position.y + _style.at(_state)._size.y);
}

sf::Vector2f GUI_Element::GetGlobalPosition() const {
    std::shared_ptr<GUI_Interface> interface(_owner);
    sf::Vector2f position = GetPosition();
    if (interface == nullptr || interface.get() == this) {
        return position; 
    }
    position += interface->GetGlobalPosition();
    if (IsControl()) {
        return position;
    }
    position.x -= interface->_scrollHorizontal;
    position.y -= interface->_scrollVertical;
    return position;
}

void GUI_Element::requireTexture(const std::string& l_name) {
    if (l_name.empty()) {
        return;
    }
    try {
        std::shared_ptr<GUI_Interface> interface(_owner);
        std::shared_ptr<GUI_Manager> guiManager(interface->GetManager());
        std::shared_ptr<SharedContext> context(guiManager->GetContext());
        std::shared_ptr<TextureManager> textureMgr(context->_textureManager);
        textureMgr->RequireResource(l_name);

    } catch(std::bad_weak_ptr& e) {
        //log
    }
}

void GUI_Element::requireFont(const std::string& l_name) {
    if (l_name.empty()) {
        return;
    }
    try {
        std::shared_ptr<GUI_Interface> interface(_owner);
        std::shared_ptr<GUI_Manager> guiManager(interface->GetManager());
        std::shared_ptr<SharedContext> context(guiManager->GetContext());
        std::shared_ptr<FontManager> fontMgr(context->_fontManager);
        fontMgr->RequireResource(l_name);

    } catch(std::bad_weak_ptr& e) {
        //log
    }
}
