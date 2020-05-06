#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "resourceManager.hpp"

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
    public:
        TextureManager(): ResourceManager("assets/textures.cfg") {}

        sf::Texture* load(const std::string& path) {
            sf::Texture* texture = new sf::Texture();
            if(!texture->loadFromFile(
                utils::GetWorkingDirectory() + path))
            {
                delete texture;
                texture = nullptr;
                std::cerr << "! Failed to load texture: " << path << std::endl;
            }
            return texture;
        }
};
