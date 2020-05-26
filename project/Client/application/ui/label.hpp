#pragma once

#include "baseElement.hpp"

class Label : public BaseElement {
public:
    explicit Label(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const std::string& l_style);

    ~Label() override;

    void Update(float l_dT) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    ElementName OnClick(const sf::Vector2i& l_mousePos) override;
    void OnRelease() override;
    void OnHover() override;
    void OnLeave() override;

private:
    void draw(const std::shared_ptr<Style>& l_style);
};
