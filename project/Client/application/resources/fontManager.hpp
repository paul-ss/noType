#pragma once

#include <SFML/Graphics/Font.hpp>

#include "resourceManager.hpp"

class FontManager : public ResourceManager<FontManager, sf::Font> {
    public:
        FontManager() : ResourceManager("assets/fonts.cfg") {}

        sf::Font* load(const std::string& l_path) {
            sf::Font* font = new sf::Font();
            if (!font->loadFromFile(utils::GetWorkingDirectory() + l_path)) {
                delete font;
                font = nullptr;
                std::cerr << "! Failed to load font: " << l_path << std::endl;
            }
            return font;
        }
};
