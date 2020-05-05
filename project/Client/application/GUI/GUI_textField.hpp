#pragma once

#include "GUI_element.hpp"

class GUI_Textfield : public GUI_Element{
public:
    explicit GUI_Textfield(const std::string& name, GUI_Interface* owner);
    ~GUI_Textfield();

    void ReadIn(std::stringstream& stream);
    void OnClick(const sf::Vector2f& mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f& mousePos);
    void OnLeave();
    void Update(float dT);
    void Draw(sf::RenderTarget* target);
};
