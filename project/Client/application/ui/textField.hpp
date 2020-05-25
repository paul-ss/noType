#pragma once

#include "baseElement.hpp"

class TextField : public BaseElement {
public:
    TextField(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style,
            const std::string& l_reference);

    ~TextField() = default;

    void Update(float l_dT) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    ElementName OnClick(const sf::Vector2i& l_mousePos) override;
    void OnRelease() override;
    void OnHover() override;
    void OnLeave() override;
};
