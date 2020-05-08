#pragma once
#include "resourceManager.hpp"
#include <SFML/Graphics/Font.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font> {
    public:
        FontManager() : ResourceManager("resources/fonts.cfg") {}

        sf::Font* Load(const std::string& l_path) {
            sf::Font* font = new sf::Font();
            if (!font->loadFromFile(utils::GetWorkingDirectory() + l_path)) {
                delete font;
                font = nullptr;
                std::cerr << "! Failed to load font: " << l_path << std::endl;
            }
            return font;
        }
};
