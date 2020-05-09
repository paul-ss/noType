#include <algorithm>
#include <string>
#include <memory>

#include "GUI_textField.hpp"
#include "utils.hpp"
#include "logger.hpp"

GUI_TextField::GUI_TextField(const std::string& l_name, std::shared_ptr<GUI_Interface> l_owner)
    : GUI_Element(l_name, GUI_ElementType::TextField , l_owner) {}

GUI_TextField::~GUI_TextField() {}

void GUI_TextField::ReadIn(std::stringstream& l_stream) {
    std::string content;
    utils::ReadQuotedString(l_stream, content);
    _visual._text.setString(content);
}

void GUI_TextField::OnClick(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Clicked);
}

void GUI_TextField::OnRelease() {}

void GUI_TextField::OnHover(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Focused);
}

void GUI_TextField::OnLeave() {
    SetState(GUI_ElementState::Neutral);
}

void GUI_TextField::Update(float l_dT) {}

void GUI_TextField::Draw(std::shared_ptr<sf::RenderTarget> l_target) {
    l_target->draw(_visual._backgroundSolid);
    if (!_style[_state]._glyph.empty()) {
        l_target->draw(_visual._glyph);
    }
    l_target->draw(_visual._text);
}
