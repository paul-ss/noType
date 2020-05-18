#pragma once

#include "baseElement.hpp"
#include "RichText.hpp"

class SmartString : public BaseElement {
public:
    SmartString(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const std::string& l_style,
            const std::string& l_reference);
    ~SmartString() = default;


    void Update(float l_dT) override;
    void Draw() override;
    void ReadIn(const std::string& l_stream) override;
    void OnClick(const sf::Vector2f& l_mousePos) override;
    void OnRelease() override;
    void OnHover(const sf::Vector2f& l_mousePos) override;
    void OnLeave() override;

public:
    std::string Validate(const char l_char);

private:
    sfe::RichText _coloredText;
    std::string _validatedBlock;
    size_t _textPosition;
    std::string _reference;
};