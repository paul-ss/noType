#pragma once

#include "RichText.hpp"

#include "baseElement.hpp"

class String : public BaseElement {
public:
    void Update(float l_dT) override;
    void Draw(std::shared_ptr<sf::RenderTarget> l_target) override;
    void ReadIn(const std::string& l_stream) override;
    void OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;
};