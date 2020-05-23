#include "textField.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

TextField::TextField(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style,
        const std::string& l_text) :
        BaseElement(l_name, l_sharedContext, l_position, l_style) {

    _visual.text.setString(l_text);
}

void TextField::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        auto styleItr = _style.find(ElementState::Neutral);
        if (styleItr == _style.end()) {
            BOOST_LOG_TRIVIAL(error) << "[String - draw] neutral state not found";
            return;
        }
        applyStyle(styleItr->second);
        renderWindow->draw(_visual.text);

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[String - draw] " << e.what();
    }
}

ElementName TextField::OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void TextField::Update([[maybe_unused]] float l_dT) {}
void TextField::ReadIn([[maybe_unused]] const std::string& l_stream) {}
void TextField::OnRelease() {}
void TextField::OnHover([[maybe_unused]] const sf::Vector2f& l_mousePos) {}
void TextField::OnLeave() {}
