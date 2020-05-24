#pragma once

#include "selbaProgressBar.hpp"
#include "Common.hpp"

#include "baseElement.hpp"

class ProgressBar : public BaseElement {
public:
    explicit ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style);

    ~ProgressBar() = default;

    void Update(float position) override;
    void Draw() override;
    void OnRelease() override;
    void ReadIn(const std::string& l_stream) override;
    ElementName OnClick(const sf::Vector2i& l_mousePos) override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;

private:
    sw::ProgressBar _progressBar;
};
