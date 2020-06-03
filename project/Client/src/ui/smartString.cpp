#include "smartString.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

#define TEXT_BLOCK_SIZE 10
#define ASCII_BACKSPACE 8
#define STRING_SIZE 60

SmartString::SmartString(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const std::string& l_style,
            const std::string& l_reference) :
            BaseElement(l_name, l_sharedContext, l_style),
            _isValid(true),
            _textPosition(0),
            _reference(l_reference) {

    
    auto numOfLines = l_reference.size() / STRING_SIZE;
    if (l_reference.size() % STRING_SIZE != 0) {
        numOfLines++;
    }

    std::string cuttedString;
    for (size_t i = 0; i < numOfLines; ++i) {
        size_t size = STRING_SIZE;
        if ((i + 1) * STRING_SIZE > l_reference.size()) {
            size = -1;
        }
        cuttedString += l_reference.substr(i * STRING_SIZE, size) + "\n";
    }
    _reference = cuttedString;

    auto styleItr = _style.find(ElementState::Neutral);
    if (styleItr == _style.end()) {
        BOOST_LOG_TRIVIAL(error) << "[smartstring - smartstring] style not found";
        return;
    }

    applyStyle(styleItr->second);
    if (_visual.text.getFont()) {
        _coloredText.setFont(*_visual.text.getFont());
    }
    _coloredText << _reference << _visual.text.getFillColor();

    _coloredText.setCharacterSize(_visual.text.getCharacterSize());
    _coloredText.setPosition(_position.x - _coloredText.getGlobalBounds().width * 0.5,
                            _position.y);
}

size_t SmartString::GetStringSize() {
    return _reference.size();
}

void SmartString::Draw() {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);
        std::shared_ptr<Window>window(sharedContext->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        renderWindow->draw(_coloredText);
    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[smartstring - draw] " << e.what();
    }
}

std::string SmartString::Validate(const char l_char) {
    try {
        if (_textPosition == _reference.size() - 1) {
          return {};
        }
        auto newline = _reference.substr(_textPosition, 1);
        if (newline == "\n") {
            _textPosition++;
        }

        sf::String beforeChar = _reference.substr(0, _textPosition);
        sf::String afterChar = _reference.substr(_textPosition + 1);
        if (l_char == ASCII_BACKSPACE && !_isValid) {
            afterChar = _reference.substr(_textPosition);
            _coloredText.clear();
            _coloredText << sf::Color::Green << beforeChar
                    << sf::Color::White <<afterChar;
            _isValid = true;
            return std::string();
        } else {
            if (!_isValid) {
                return std::string();
            }
            if (l_char == _reference[_textPosition]) {
                _validatedBlock.push_back(l_char);
                _textPosition++;
                _coloredText.clear();
                _coloredText << sf::Color::Green << beforeChar
                        << sf::Color::Green << l_char
                        << sf::Color::White << afterChar;
                if (_validatedBlock.size() >= TEXT_BLOCK_SIZE || _textPosition == _reference.size() - 1) {
                    std::string temp = _validatedBlock;
                    _validatedBlock = std::string();
                    return temp;
                }
                return std::string();
            } else {
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
        }

    } catch (std::out_of_range& e) {
        BOOST_LOG_TRIVIAL(error) << "[validate - smartstring] " << e.what();
    }
    return std::string();
}

ElementName SmartString::OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) {
    return ElementName::None;
}

void SmartString::ReadIn([[maybe_unused]] const std::string& l_stream) {}
void SmartString::OnHover() {}
void SmartString::OnLeave() {}
void SmartString::Update([[maybe_unused]] float l_dT) {}
