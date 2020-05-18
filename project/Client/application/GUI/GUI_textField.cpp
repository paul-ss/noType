#include "GUI_textfield.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"

GUI_Textfield::GUI_Textfield(const std::string& name, GUI_Interface* owner) :
        GUI_Element(name, GUI_ElementType::Textfield , owner) {}

GUI_Textfield::~GUI_Textfield() {}

void GUI_Textfield::ReadIn(std::stringstream& stream) {
    std::string content;
    utils::ReadQuotedString(stream, content);
    _visual._text.setString(content);
}

void GUI_Textfield::OnClick(const sf::Vector2f& mousePos) {
    SetState(GUI_ElementState::Clicked);
}

void GUI_Textfield::OnRelease() {}

void GUI_Textfield::OnHover(const sf::Vector2f& mousePos) {
    SetState(GUI_ElementState::Focused);
}

void GUI_Textfield::OnLeave() {
    SetState(GUI_ElementState::Neutral);
}

void GUI_Textfield::Update(float dT) {}

void GUI_Textfield::Draw(sf::RenderTarget* target) {
    target->draw(_visual._backgroundSolid);
    if (_style[_state]._glyph != "") {
        target->draw(_visual._glyph);
    }
    target->draw(_visual._text);
}
