#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "resourceManager.hpp"

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
    public:
        TextureManager(): ResourceManager("assets/textures.cfg") {}

        sf::Texture* load(const std::string& path) {
            sf::Texture* texture = new sf::Texture();
            if(!texture->loadFromFile(std::filesystem::absolute(path))) {
                delete texture;
                texture = nullptr;
                BOOST_LOG_TRIVIAL(error) << "Failed to load texture file: " << path;
            }
            return texture;
        }
};
