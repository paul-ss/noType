#pragma once

#include <unordered_map>
#include <memory>

#include <SFML/Graphics.hpp>

struct SharedContext;

enum class ElementState {
    Neutral,
    Focused,
    Clicked,
    Hover,
    Release,
    TextEntered
};

// Contains all ui instances.
enum class ElementName {
    None = -1,
    Filler,
    IntroText,
    PlayButton,
    QuitButton,
    TimeToStart,
    MenuButton,
    LeaderBar,
    PlayerBar,
    PlayerPosition,
    PlayerPositionText,
    AverageSpeed,
    AverageSpeedText,
    SmartString,
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

class BaseElement {
public:
    BaseElement(const ElementName l_name, std::weak_ptr<SharedContext> l_sharedContext,
            const std::string& l_style);

    virtual ~BaseElement() = default;

    virtual void Draw() = 0;
    virtual void Update([[maybe_unused]] float l_dT) = 0;
    virtual void ReadIn([[maybe_unused]] const std::string& l_stream) = 0;
    virtual ElementName OnClick([[maybe_unused]] const sf::Vector2i& l_mousePos) = 0;
    virtual void OnRelease() = 0;
    virtual void OnHover() = 0;
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
    void loadPosition(const ElementName l_element, const std::string& l_path);

protected:
    ElementName _name;
    sf::Vector2f _position;
    ElementStyles _style;
    Visual _visual;
    ElementState _state;
    std::weak_ptr<SharedContext> _sharedContext;
};
