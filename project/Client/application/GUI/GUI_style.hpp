#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <string>

enum class GUI_ElementState{ Neutral, Focused, Clicked };

struct GUI_Style {
    explicit GUI_Style(): _textSize(12), _textCenterOrigin(false),
        _backgroundImageColor(255,255,255,255) {
        sf::Color none = sf::Color(0, 0, 0, 0);
        _backgroundColor = none;
        _elementColor = none;
        _textColor = none;
    }

    sf::Vector2f _size; // Element size.

    // Background properties.
    sf::Color _backgroundColor;
    sf::Color _elementColor;
    std::string _backgroundImage;
    sf::Color _backgroundImageColor;

    // Text properties.
    sf::Color _textColor;
    std::string _textFont;
    sf::Vector2f _textPadding;
    unsigned int _textSize;
    bool _textCenterOrigin;

    // Glyph properties.
    std::string _glyph;
    sf::Vector2f _glyphPadding;
};

struct GUI_Visual {
    sf::RectangleShape _backgroundSolid;
    sf::Sprite _backgroundImage;
    sf::Sprite _glyph;
    sf::Text _text;
};
