#pragma once

#include "selbaProgressBar.hpp"
#include "Common.hpp"

#include "baseElement.hpp"

class ProgressBar : public BaseElement {
public:
    ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style,
            const std::string& l_reference);

    ~ProgressBar() = default;

    void Update(float l_dT) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    ElementName OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;

private:
    sw::ProgressBar _progressBar;
};