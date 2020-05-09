#include <algorithm>
#include <iostream>

#include "GUI_label.hpp"
#include "utils.hpp"

GUI_Label::GUI_Label(const std::string& l_name, GUI_Interface* l_owner)
    : GUI_Element(l_name, GUI_ElementType::Label, l_owner) {}

void GUI_Label::ReadIn(std::stringstream& l_stream) {
    std::string content;
    utils::ReadQuotedString(l_stream, content);
    _visual._text.setString(content);
}

void GUI_Label::OnClick(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Clicked);
}
void GUI_Label::OnRelease() {
    SetState(GUI_ElementState::Neutral);
}
void GUI_Label::OnHover(const sf::Vector2f& l_mousePos) {
    SetState(GUI_ElementState::Focused);
}
void GUI_Label::OnLeave() {
    SetState(GUI_ElementState::Neutral);
}
void GUI_Label::Update(float l_dT) {}
void GUI_Label::Draw(std::weak_ptr<sf::RenderTarget> l_target) {
    auto target = l_target.lock();
    if (!target) {
        BOOST_LOG_TRIVIAL(error) << "Cannot render Label";
        return;
    }
    target->draw(_visual._backgroundSolid);
    if (_style[_state]._glyph != "") { target->draw(_visual._glyph); }
    target->draw(_visual._text);
}
