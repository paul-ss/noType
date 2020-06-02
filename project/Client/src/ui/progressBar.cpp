#include "progressBar.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

void ProgressBar::applyStyleProgressBar() {
    _progressBar.setPosition(_position.x, _position.y);
    _progressBar.setShowBackgroundAndFrame(true);
    _progressBar.setSize(_visual.backgroundSolid.getSize());
    _progressBar.setOrigin(_progressBar.getLocalBounds().width * 0.5, 0);
    _progressBar.setColor(_visual.backgroundSolid.getFillColor());
}

ProgressBar::ProgressBar(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const std::string& l_style) :
        BaseElement(l_name, l_sharedContext, l_style) {

    auto styleItr = _style.find(ElementState::Neutral);
    if (styleItr == _style.end()) {
        //log
        return;
    }
    applyStyle(styleItr->second);
    applyStyleProgressBar();
}

ElementName ProgressBar::OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void ProgressBar::Update(float l_position) {
    _progressBar.setRatio(l_position);
}

void ProgressBar::SetPlayerStyle() {
    _state = ElementState::Player;
}

void ProgressBar::SetEnemyStyle() {
    _state = ElementState::Neutral;
}

void ProgressBar::Draw() {
    if (_state == ElementState::Neutral) {
        auto styleItr = _style.find(ElementState::Neutral);
        if (styleItr == _style.end()) {
            //log
            return;
        }
        applyStyle(styleItr->second);
        applyStyleProgressBar();
        draw(styleItr->second);
    }
    if (_state == ElementState::Player) {
        auto styleItr = _style.find(ElementState::Player);
        if (styleItr == _style.end()) {
            //log
            return;
        }
        applyStyle(styleItr->second);
        applyStyleProgressBar();
        draw(styleItr->second);
    }
}

void ProgressBar::draw(const std::shared_ptr<Style>& l_style) {
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
