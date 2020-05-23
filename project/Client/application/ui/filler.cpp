#include "filler.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

Filler::Filler(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style) :
        BaseElement(l_name, l_sharedContext, l_position, l_style) {}

Filler::~Filler() {}

void Filler::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        auto styleItr = _style.find(ElementState::Neutral);
        if (styleItr == _style.end()) {
            BOOST_LOG_TRIVIAL(error) << "[Filler - filler] Neutral style not found";
            return;
        }
        applyStyle(styleItr->second);

        renderWindow->draw(_visual.backgroundSolid);
        if (!styleItr->second->backgroundImage.empty()) {
            renderWindow->draw(_visual.backgroundImage);
        }

        if (!styleItr->second->backgroundImage.empty()) {
            renderWindow->draw(_visual.text);
        }

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[Filler - draw] " << e.what();
    }
}

ElementName Filler::OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void Filler::Update([[maybe_unused]] float l_dT) {}
void Filler::ReadIn([[maybe_unused]] const std::string& l_stream) {}
void Filler::OnRelease() {}
void Filler::OnHover([[maybe_unused]] const sf::Vector2f& l_mousePos) {}
void Filler::OnLeave() {}
