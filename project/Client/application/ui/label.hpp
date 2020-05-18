#pragma once

#include "baseElement.hpp"

class Label : public BaseElement {
public:
    explicit Label(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position, const std::string& l_style);

    ~Label() override;

    void Update(float l_dT) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    void OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;

private:
    void draw(const std::shared_ptr<Style>& l_style);
};
