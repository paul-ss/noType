#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "resourceManager.hpp"

#define TEXTURES_FILE_PATH "assets/textures.json"

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
public:
    TextureManager(): ResourceManager(TEXTURES_FILE_PATH, "textures") {}

    std::shared_ptr<sf::Texture> load(const std::string& l_path) {
        auto texture = std::make_shared<sf::Texture>();
        if(!texture->loadFromFile(std::filesystem::absolute(l_path))) {
            texture.reset();
            texture = {};
            BOOST_LOG_TRIVIAL(error) << "Failed to load texture file: " << l_path;
        }
        return texture;
    }
};
