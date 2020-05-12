#pragma once

#include "baseElement.hpp"

#define BUTTON_STYLE_PATH "assets/media/styles"

class Button : public BaseElement {
public:
    Button() : BaseElement(BUTTON_STYLE_PATH);
    ~Button();
    void Draw(std::shared_ptr<sf::RenderTarget> l_target) override;
    void ReadIn(std::string& l_stream) override;
    void OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;
    void Update(float l_dT) override;
    void Draw(std::shared_ptr<sf::RenderTarget> l_target) override;
};
