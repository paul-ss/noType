#pragma once

#include "baseElement.hpp"

class Button : public BaseElement {
public:
    Button(std::weak_ptr<SharedContext> l_sharedContext);
    ~Button() = default;

    void Update(float l_dT) override;
    void Draw(std::shared_ptr<sf::RenderTarget> l_target) override;

protected:
    void readIn(const std::string& l_stream) override;
    void onClick(const sf::Vector2f& l_mousePos) override;
    void onRelease() override;
    void onHover(const sf::Vector2f& l_mousePos) override;
    void onLeave() override;
    void loadStyle(const std::string& l_path) override;
};
