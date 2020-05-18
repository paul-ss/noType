#include "baseElement.hpp"
#include "logger.hpp"

String::String(std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style,
        const std::string& l_text) :
        BaseElement(l_sharedContext, l_position, l_style) {

    _visual.text.setString(l_text);
    }

void String::Update(float l_dT) {}

void String::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        renderWindow->draw(_visual.text);

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[String - draw] " << e.what();
}

void String::ReadIn(const std::string& l_stream) {}
void String::OnClick(const sf::Vector2f& l_mousePos) {}
void String::OnRelease() {}
void String::OnHover(const sf::Vector2f& l_mousePos) {}
void String::OnLeave() {}
