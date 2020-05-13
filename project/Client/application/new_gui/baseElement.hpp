#pragma once

#include "sharedContext.hpp"

enum class GUI_ElementType {
    Label,
    Button,
    TextField
};

enum class GUI_ElementState {
    Neutral,
    Focused,
    Clicked,
    Hover,
    Release,
    TextEntered
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

using ElementStyles = std::unordered_map<GUI_ElementState, std::shared_ptr<GUI_Style>>;
struct SharedContext;
class BaseElement {
public:
    BaseElement(const std::string& l_path, std::weak_ptr<SharedContext> l_sharedContext) :
            _sharedContext(l_sharedContext) {}
    ~BaseElement() = default;

    virtual void Draw(std::shared_ptr<sf::RenderTarget> l_target);
    virtual void Update(float l_dT);

protected:
    virtual void loadStyle(const std::string& l_path) = 0;
    virtual void ReadIn(const std::string& l_stream) = 0;
    virtual void OnClick(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnRelease() = 0;
    virtual void OnHover(const sf::Vector2f& l_mousePos) = 0;
    virtual void OnLeave();

protected:
    std::string _name;
    sf::Vector2f _position;
    ElementStyles _style;
    GUI_Visual _visual;
    GUI_ElementType _type;
    GUI_ElementState _state;

    std::weak_ptr<SharedContext> _sharedContext;
    std::shared_ptr<sf::Font> _font;
    std::shared_ptr<sf::Texture> _texture;

    bool _needsRedraw;
    bool _active;
    bool _isControl;
};
