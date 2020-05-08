#include <algorithm>
#include <iostream>

#include "GUI_textField.hpp"
#include "utils.hpp"

GUI_Textfield::GUI_Textfield(const std::string& l_name, GUI_Interface* l_owner)
    : GUI_Element(l_name, GUI_ElementType::Textfield , l_owner) {}

GUI_Textfield::~GUI_Textfield() {}

void GUI_Textfield::ReadIn(std::stringstream& l_stream) {
    std::string content;
    utils::ReadQuotedString(l_stream, content);
    _visual._text.setString(content);
}

void GUI_Textfield::OnClick(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Clicked);
}

void GUI_Textfield::OnRelease() {}

void GUI_Textfield::OnHover(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Focused);
}

void GUI_Textfield::OnLeave() {
    SetState(GUI_ElementState::Neutral);
}
void GUI_Textfield::Update(float l_dT) {}

void GUI_Textfield::Draw(sf::RenderTarget* l_target) {
    l_target->draw(_visual._backgroundSolid);
    if (!_style[_state]._glyph.empty()) {
        l_target->draw(_visual._glyph);
    }
    l_target->draw(_visual._text);
}
