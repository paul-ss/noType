#include "progressBar.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

ProgressBar::ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const std::string& l_style) :
        BaseElement(l_name, l_sharedContext, l_style) {

    auto styleItr = _style.find(ElementState::Neutral);
    if (styleItr == _style.end()) {
        //log
        return;
    }
    applyStyle(styleItr->second);

    _progressBar.setPosition(_position.x, _position.y);
    _progressBar.setShowBackgroundAndFrame(true);
    _progressBar.setSize(_visual.backgroundSolid.getSize());
    _progressBar.setOrigin(_progressBar.getLocalBounds().width * 0.5, 0);
    _progressBar.setColor(_visual.backgroundSolid.getFillColor());
}

ElementName ProgressBar::OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void ProgressBar::OnRelease() {}

void ProgressBar::Update(float l_position) {
    _progressBar.setRatio(l_position);
}
void ProgressBar::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        renderWindow->draw(_progressBar);
        renderWindow->draw(_visual.text);

    } catch (std::bad_weak_ptr& e) {
        //log
    }
}
void ProgressBar::ReadIn([[maybe_unused]] const std::string& l_stream) {}
void ProgressBar::OnHover() {}
void ProgressBar::OnLeave() {}
