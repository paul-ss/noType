#pragma once

#include "selbaProgressBar.hpp"
#include "Common.hpp"

#include "baseElement.hpp"

class ProgressBar : public BaseElement {
public:
    explicit ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const std::string& l_style);

    ~ProgressBar() = default;

    void Update(float position) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    void OnHover() override;
    void OnLeave() override;
    ElementName OnClick(const sf::Vector2i& l_mousePos) override;

public:
    void SetPlayerStyle();
    void SetEnemyStyle();

private:
    void draw();
    void applyStyleProgressBar();

private:
    sw::ProgressBar _progressBar;
};
