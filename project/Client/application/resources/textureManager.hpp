#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "resourceManager.hpp"

#define TEXTURES_FILE_PATH "assets/textures.cfg"

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
    public:
        TextureManager(): ResourceManager(TEXTURES_FILE_PATH) {}

        sf::Texture* load(const std::string& l_path) {
            sf::Texture* texture = new sf::Texture();
            if(!texture->loadFromFile(std::filesystem::absolute(l_path))) {
                delete texture;
                texture = nullptr;
                BOOST_LOG_TRIVIAL(error) << "Failed to load texture file: " << l_path;
            }
            return texture;
        }
};
