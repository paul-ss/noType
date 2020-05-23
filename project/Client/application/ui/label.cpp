#include "label.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

Label::Label(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position, const std::string& l_style) :
        BaseElement(l_name, l_sharedContext, l_position, l_style) {}

Label::~Label() {}

ElementName Label::OnClick(const sf::Vector2i& l_mousePos) {
    auto styleItr = _style.find(ElementState::Neutral);
    if (styleItr == _style.end()) {
        //log
    }
    if (styleItr->second->isFullScreen) {
        return ElementName::None;
    }
    float halfX = GetSize().x / 2.0f;
    float halfY = GetSize().y / 2.0f;
    if (l_mousePos.x >= GetPosition().x - halfX &&
        l_mousePos.x <= GetPosition().x + halfX &&
        l_mousePos.y >= GetPosition().y - halfY &&
        l_mousePos.y <= GetPosition().y + halfY) {
        _state = ElementState::Clicked;
        return _name;
    }
    return ElementName::None;
}

void Label::OnRelease() {
    _state = ElementState::Hover;
}

void Label::OnHover([[maybe_unused]] const sf::Vector2f& l_mousePos) {
    _state = ElementState::Hover;
}

void Label::OnLeave() {
    _state = ElementState::Neutral;
}

void Label::Update([[maybe_unused]] float l_dT) {}

void Label::draw(const std::shared_ptr<Style>& l_style) {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        renderWindow->draw(_visual.backgroundSolid);
        if (!l_style->backgroundImage.empty()) {
            renderWindow->draw(_visual.backgroundImage);
        }

        // Text
        if (!l_style->textFont.empty()) {
            renderWindow->draw(_visual.text);
        }

        // Glyph
        if (!l_style->glyph.empty()) {
            renderWindow->draw(_visual.glyph);
        }
    } catch (std::bad_weak_ptr& e) {
        //log
    }
}

void Label::Draw() {
    if (_state == ElementState::Neutral) {
        auto styleItr = _style.find(ElementState::Neutral);
        if (styleItr == _style.end()) {
            //log
            return;
        }
        applyStyle(styleItr->second);
        draw(styleItr->second);
    }

    if (_state == ElementState::Clicked) {
        auto styleItr = _style.find(ElementState::Clicked);
        if (styleItr == _style.end()) {
            //log
            return;
        }
        applyStyle(styleItr->second);
        draw(styleItr->second);
        return;
    }

    if (_state == ElementState::Hover) {
        auto styleItr = _style.find(ElementState::Hover);
        if (styleItr == _style.end()) {
            //log
            return;
        }
        applyStyle(styleItr->second);
        draw(styleItr->second);
        return;
    }
    //log
}

void Label::ReadIn([[maybe_unused]] const std::string& l_stream) {}
