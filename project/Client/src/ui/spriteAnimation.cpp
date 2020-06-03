#include "spriteAnimation.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

SpriteAnimation::SpriteAnimation(std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const sf::Vector2i& l_size,
        const std::string& l_file) :
        _animatedSprite(sf::seconds(0.05), true, false),
        _sharedContext(l_sharedContext)
        {

    std::shared_ptr<SharedContext>context(_sharedContext);
    std::shared_ptr<Window>window(context->window);
    std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

    addFrame(l_file, l_size);
    _animatedSprite.setPosition(l_position);
    _animatedSprite.setLooped(true);
    _animatedSprite.play(_animation);
    _animatedSprite.setScale(20,20);
    _animatedSprite.setOrigin(_animatedSprite.getLocalBounds().width * 0.5, _animatedSprite.getLocalBounds().height * 0.5);
}

void SpriteAnimation::Draw() {
    try {
        std::shared_ptr<SharedContext>context(_sharedContext);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
        renderWindow->draw(_animatedSprite);

    } catch (const std::bad_weak_ptr& e) {
        //log
    }
}

void SpriteAnimation::addFrame(const std::string& l_file, const sf::Vector2i& l_size) {
    try {
        std::shared_ptr<SharedContext>context(_sharedContext);
        std::shared_ptr<TextureManager>tMgr(context->textureManager);
        if (tMgr->RequireResource(l_file)) {
            std::shared_ptr<sf::Texture>texture(tMgr->GetResource(l_file));
            size_t rows = texture->getSize().x / l_size.x;
            size_t cols = texture->getSize().y / l_size.y;
            _animation.setSpriteSheet(*texture);
            for (size_t i = 0; i < cols; ++i) {
                for (size_t j = 0; j < rows; ++j) {
                    _animation.addFrame(sf::IntRect(l_size.x * j, l_size.y * i, l_size.x, l_size.y));
                }
            }
        } else {
            //log
            return;
        }

    } catch (const std::bad_weak_ptr& e) {
        //log
    }
}

void SpriteAnimation::Update(sf::Time l_time) {
    _animatedSprite.update(l_time);
}
