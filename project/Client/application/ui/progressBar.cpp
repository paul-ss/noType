#include "progressBar.hpp"
#include "logger.hpp"

ProgressBar::ProgressBar(std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style,
        const std::string& l_reference) :
        BaseElement(l_sharedContext, l_position, l_style) {}

void Update(float l_dT) {}
void Draw() {}
void ReadIn(const std::string& l_stream) {}
void OnClick(const sf::Vector2f& l_mousePos) {}
void OnRelease() {}
void OnHover(const sf::Vector2f& l_mousePos) {}
void OnLeave() {}