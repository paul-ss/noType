#pragma once

#include "GUI_element.hpp"

class GUI_TextField : public GUI_Element {
public:
    GUI_TextField(const std::string& l_name);
    ~GUI_TextField();

    void ReadIn(std::stringstream& l_stream);
    void OnClick(const sf::Vector2f& l_mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f& l_mousePos);
    void OnLeave();
    void Update(float l_dT);
    void Draw(std::shared_ptr<sf::RenderTarget> l_target);
};
