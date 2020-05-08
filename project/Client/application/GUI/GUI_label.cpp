#include "GUI_label.hpp"

#include <algorithm>
#include <iostream>

#include "utils.hpp"

GUI_Label::GUI_Label(const std::string& name, GUI_Interface* owner) :
        GUI_Element(name, GUI_ElementType::Label, owner) {}

GUI_Label::~GUI_Label() {}

void GUI_Label::ReadIn(std::stringstream& stream) {
    std::string content;
    utils::ReadQuotedString(stream, content);
    _visual._text.setString(content);
}

void GUI_Label::OnClick(const sf::Vector2f& mousePos) {
    SetState(GUI_ElementState::Clicked);
}
void GUI_Label::OnRelease() {
    SetState(GUI_ElementState::Neutral);
}
void GUI_Label::OnHover(const sf::Vector2f& mousePos) {
    SetState(GUI_ElementState::Focused);
}
void GUI_Label::OnLeave() {
    SetState(GUI_ElementState::Neutral);
}
void GUI_Label::Update(float dT) {}

void GUI_Label::Draw(sf::RenderTarget* target) {
    target->draw(_visual._backgroundSolid);
    if (_style[_state]._glyph != "") {
        target->draw(_visual._glyph);
    }
    target->draw(_visual._text);
}
