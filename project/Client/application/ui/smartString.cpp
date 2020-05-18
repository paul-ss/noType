#include "smartString.hpp"
#include "logger.hpp"

#define TEXT_BLOCK 10
#define ASCII_BACKSPACE 8

SmartString::SmartString(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style,
            const std::string& l_reference) :
            BaseElement(l_sharedContext, l_position, l_style),
            _isValid(true),
            _textPosition(0),
            _reference(l_reference) {

    auto styleItr = _style.find(ElementState::Neutral);
        if (styleItr == _style.end()) {
            //log
            return;
        }
    applyStyle(styleItr->second);
    if (_visual.text.getFont()) {
        _coloredText.setFont(*_visual.text.getFont());
    }
    _coloredText << l_reference << _visual.text.getFillColor();

        sf::Vector2f txtSz(_coloredText.getGlobalBounds().width,
                _coloredText.getGlobalBounds().height);

    _coloredText.setCharacterSize(_visual.text.getCharacterSize());
    _coloredText.setPosition(l_position.x - _coloredText.getGlobalBounds().width * 0.5,
                            l_position.y);
}

void SmartString::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        //sf::Vector2f txtSz(_coloredText.getGlobalBounds().width,
        //        _coloredText.getGlobalBounds().height);
        //_visual.backgroundSolid.setSize(txtSz);

        renderWindow->draw(_coloredText);
    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[smartString - draw] " << e.what();
    }
}

std::string SmartString::Validate(const char l_char) {
    try {
        sf::String beforeChar = _reference.substr(0, _textPosition);
        sf::String afterChar = _reference.substr(_textPosition + 1);
        if (l_char == ASCII_BACKSPACE && !_isValid) {
            std::cout <<"DELETE\n";
            afterChar = _reference.substr(_textPosition);
            _coloredText.clear();
            _coloredText << sf::Color::Green << beforeChar <<
                    sf::Color::White <<afterChar;
            _isValid = true;
            return std::string();
        } else {
            if (!_isValid) {
                std::cout <<"NOTHING\n";
                return std::string();
            }

            if (l_char != _reference[_textPosition]) {
                std::cout <<"INVALID\n";
                std::cout << l_char << " char invalid\n";
                std::cout << _reference[_textPosition] << " text position\n";
                auto falseChar = _reference.substr(_textPosition, 1);
                if (falseChar == " ") {
                    falseChar = "_";
                }
                _coloredText.clear();
                _coloredText << sf::Color::Green << beforeChar
                        << sf::Color::Red << falseChar
                        << sf::Color::White << afterChar;
                _isValid = false;
                return std::string();
            }
            std::cout <<"VALID\n";
            std::cout << l_char << "\n";
            std::cout << _reference[_textPosition] << "\n";
            _validatedBlock.push_back(l_char);
            _textPosition++;
            _coloredText.clear();
            _coloredText << sf::Color::Green << beforeChar
                    << sf::Color::Green << l_char
                    << sf::Color::White << afterChar;
            if (_validatedBlock.size() <= TEXT_BLOCK) {
                return std::string();
            }
            return _validatedBlock;
        }
    } catch (std::out_of_range& e) {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
    return std::string();
}

void SmartString::ReadIn(const std::string& l_stream) {}
void SmartString::OnClick(const sf::Vector2f& l_mousePos) {}
void SmartString::OnRelease() {}
void SmartString::OnHover(const sf::Vector2f& l_mousePos) {}
void SmartString::OnLeave() {}
void SmartString::Update(float l_dT) {}