#pragma once

#include "baseElement.hpp"

class Button : public BaseElement {
public:
    Button(std::weak_ptr<SharedContext> l_sharedContext);
    ~Button() = default;
    void ReadIn(const std::string& l_stream) override;
    void OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;
    void Update(float l_dT) override;
    void Draw(std::shared_ptr<sf::RenderTarget> l_target) override;

protected:
    void loadStyle(const std::string& l_path) override;
};
