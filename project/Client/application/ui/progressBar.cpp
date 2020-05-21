#include "progressBar.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

ProgressBar::ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style,
        const std::string& l_reference) :
        BaseElement(l_name, l_sharedContext, l_position, l_style) {}

ElementName OnClick(const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void Update([[maybe_unused]] float l_dT) {}
void Draw() {}
void ReadIn([[maybe_unused]] const std::string& l_stream) {}
void OnRelease() {}
void OnHover([[maybe_unused]] const sf::Vector2f& l_mousePos) {}
void OnLeave() {}
