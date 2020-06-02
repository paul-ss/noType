#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "animatedSprite.hpp"

struct SharedContext;

class SpriteAnimation {
public:
    explicit SpriteAnimation(std::weak_ptr<SharedContext> l_sharedContext,
            const sf::Vector2f& l_position,
            const sf::Vector2i& l_size,
            const std::string& l_file);

    ~SpriteAnimation() = default;

    void Draw();
    void Update(sf::Time l_time);

private:
    void addFrame(const std::string& l_file, const sf::Vector2i& l_size);

private:
    AnimatedSprite _animatedSprite;
    Animation _animation;
    std::weak_ptr<SharedContext> _sharedContext;
};