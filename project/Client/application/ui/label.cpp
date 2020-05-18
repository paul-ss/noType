#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "label.hpp"
#include "logger.hpp"

Label::Label(std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position, const std::string& l_style) :
        BaseElement(l_sharedContext, l_position, l_style) {}

Label::~Label() {}

void Label::ReadIn(const std::string& l_stream) {}

void Label::OnClick(const sf::Vector2f& l_mousePos) {
    _state = ElementState::Clicked;
}

void Label::OnRelease() {
    _state = ElementState::Hover;
}

void Label::OnHover(const sf::Vector2f& l_mousePos) {
    _state = ElementState::Hover;
}

void Label::OnLeave() {
    _state = ElementState::Neutral;
}

void Label::Update(float l_dT) {}

void Label::draw(const std::shared_ptr<Style>& l_style) {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        // Background
        //std::cout << _visual.backgroundSolid.getSize().x << "sx\n";
        //std::cout << _visual.backgroundSolid.getSize().y << "sy\n";
        //std::cout << _visual.backgroundSolid.getPosition().x << "pos\n";
        //std::cout << _visual.backgroundSolid.getPosition().x << "pos\n";
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
