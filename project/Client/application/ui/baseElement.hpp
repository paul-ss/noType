#pragma once

#include "sharedContext.hpp"

enum class ElementState {
    Neutral,
    Focused,
    Clicked,
    Hover,
    Release,
    TextEntered
};

struct Style {
    explicit Style():
            backgroundImageColor(255, 255, 255, 255),
            textSize(12),
            textCenterOrigin(false) {

        sf::Color none = sf::Color(0, 0, 0, 0);
        backgroundColor = none;
        textColor = none;
    }

    sf::Vector2f size;
    bool isFullScreen;

    // Background properties.
    sf::Color backgroundColor;
    std::string backgroundImage;
    sf::Color backgroundImageColor;

    // Text properties.
    sf::Color textColor;
    std::string textFont;
    sf::Vector2f textPadding;
    unsigned int textSize;
    bool textCenterOrigin;

    // Glyph properties.
    std::string glyph;
    sf::Vector2f glyphPadding;
};

struct Visual {
    sf::RectangleShape backgroundSolid;
    sf::Sprite backgroundImage;
    sf::Sprite glyph;
    sf::Text text;
};

using ElementStyles = std::unordered_map<ElementState, std::shared_ptr<Style>>;
struct SharedContext;
class BaseElement {
public:
    BaseElement(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position, const std::string& l_style);

    virtual ~BaseElement() = default;

    virtual void Draw() = 0;
    virtual void Update(float l_dT) = 0;
    virtual void ReadIn(const std::string& l_stream) = 0;
    virtual void OnClick(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnRelease() = 0;
    virtual void OnHover(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnLeave() = 0;

public:
    sf::Vector2f GetSize() const;
    sf::Vector2f GetPosition() const;
    void SetPosition(const sf::Vector2f& l_position);
    ElementState GetState() const;
    void SetText(const std::string& l_text);

protected:
    void applyStyle(const std::shared_ptr<Style>& l_style);
    void loadStyle(const std::string& l_path);

protected:
    sf::Vector2f _position;
    ElementStyles _style;
    Visual _visual;
    ElementState _state;

    std::weak_ptr<SharedContext> _sharedContext;

    bool _needsRedraw;
    bool _active;
    bool _isControl;
};
