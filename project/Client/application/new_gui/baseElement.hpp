#pragma once

#include "eventManager.hpp"

enum class GUI_ElementType {
    Label,
    Button,
    TextField
};

enum class GUI_ElementState{
    Neutral,
    Focused,
    Clicked
};

enum class GUI_EventType {
    None,
    Click,
    Release,
    Hover,
    Leave
};

struct ClickCoordinates {
    float x, y;
};

struct GUI_Event {
    GUI_EventType type;
    std::string element;
    std::string interface;
    union {
        ClickCoordinates clickCoords;
    };
};

struct GUI_Style {
    explicit GUI_Style():
            backgroundImageColor(255, 255, 255, 255),
            textSize(12),
            textCenterOrigin(false) {

        sf::Color none = sf::Color(0, 0, 0, 0);
        backgroundColor = none;
        elementColor = none;
        textColor = none;
    }

    sf::Vector2f size; // Element size.

    // Background properties.
    sf::Color backgroundColor;
    sf::Color elementColor;
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

struct GUI_Visual {
    sf::RectangleShape backgroundSolid;
    sf::Sprite backgroundImage;
    sf::Sprite glyph;
    sf::Text text;
};

class BaseElement {
public:
    BaseElement(const std::string& l_path, std::weak_ptr<EventManager> l_eMgr) : _eventManager(l_eMgr) {
        loadStyle(l_path);
}
    ~BaseElement() = default;

    virtual void Draw(std::shared_ptr<sf::RenderTarget> l_target) = 0;
    virtual void ReadIn(cosnt std::string& l_stream) = 0;
    virtual void OnClick(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnRelease() = 0;
    virtual void OnHover(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnLeave() = 0;
    virtual void Update(float l_dT) = 0;

protected:
    void loadStyle(const std::string& l_path);

protected:
    std::string _name;
    sf::Vector2f _position;
    ElementStyles _style;
    GUI_Visual _visual;
    GUI_ElementType _type;
    GUI_ElementState _state;

    std::weak_ptr<EventManager> _eventManager;
    std::shared_ptr<sf::Font> _font;
    std::shared_ptr<sf::Texture> _texture;

    bool _needsRedraw;
    bool _active;
    bool _isControl;
};
