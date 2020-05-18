#include "smartString.hpp"
#include "logger.hpp"

#define TEXT_BLOCK 8

SmartString::SmartString(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style,
            const std::string& l_reference) :
            BaseElement(l_sharedContext, l_position, l_style),
            _coloredText(*_visual.text.getFont()),
            _textPosition(0),
            _reference(l_reference) {

    _coloredText << l_reference << _visual.text.getColor();
    _coloredText.setCharacterSize(_visual.text.getCharacterSize());
    _coloredText.setPosition(_visual.text.getPosition());
}

void SmartString::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        sf::Vector2f txtSz(_coloredText.getGlobalBounds().width,
                _coloredText.getGlobalBounds().height);
        _visual.backgroundSolid.setSize(txtSz);

        renderWindow->draw(_coloredText);
    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[smartString - draw] " << e.what();
    }
}

std::string SmartString::Validate(const char l_char) {
    std::string beforeVld = _reference.substr(0, _textPosition);
    std::string afterVld = _reference.substr(_textPosition + 1, _reference.size());
    if (l_char == _reference[_textPosition]) {
        _textPosition++;
        _validatedBlock.push_back(l_char);
        _coloredText << beforeVld << sf::Color::Green <<
                    l_char << sf::Color::Red <<
                    afterVld << _visual.text.getColor();
    }
    _coloredText << beforeVld << sf::Color::Green <<
                l_char << sf::Color::Green <<
                afterVld << _visual.text.getColor();
    if (_validatedBlock.size() <= TEXT_BLOCK) {
        return std::string();
    }
    return _validatedBlock;
}

void SmartString::ReadIn(const std::string& l_stream) {}
void SmartString::OnClick(const sf::Vector2f& l_mousePos) {}
void SmartString::OnRelease() {}
void SmartString::OnHover(const sf::Vector2f& l_mousePos) {}
void SmartString::OnLeave() {}
void SmartString::Update(float l_dT) {}