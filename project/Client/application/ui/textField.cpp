#include "textField.hpp"
#include "logger.hpp"

TextField::TextField(std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style,
        const std::string& l_text) :
        BaseElement(l_sharedContext, l_position, l_style) {

    _visual.text.setString(l_text);
}

void TextField::Update(float l_dT) {}

void TextField::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        renderWindow->draw(_visual.text);

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[String - draw] " << e.what();
    }
}

void TextField::ReadIn(const std::string& l_stream) {}
void TextField::OnClick(const sf::Vector2f& l_mousePos) {}
void TextField::OnRelease() {}
void TextField::OnHover(const sf::Vector2f& l_mousePos) {}
void TextField::OnLeave() {}
