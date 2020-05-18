#pragma once
#include "resourceManager.hpp"
#include <SFML/Graphics/Texture.hpp>

class TextureManager: public ResourceManager<TextureManager, sf::Texture> {
    public:
        TextureManager(): ResourceManager("resources/textures.cfg") {}

        sf::Texture* Load(const std::string& path) {
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
