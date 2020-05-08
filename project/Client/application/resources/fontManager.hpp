#pragma once

#include <SFML/Graphics/Font.hpp>

#include "resourceManager.hpp"

#define FONTS_FILE_PATH "assets/fonts.cfg"

class FontManager : public ResourceManager<FontManager, sf::Font> {
    public:
        FontManager() : ResourceManager(FONTS_FILE_PATH) {}

        sf::Font* load(const std::string& l_path) {
            sf::Font* font = new sf::Font();
            if (!font->loadFromFile(std::filesystem::absolute(l_path))) {
                delete font;
                font = nullptr;
                BOOST_LOG_TRIVIAL(error) << "Failed to load font file: " << l_path;
            }
            return font;
        }
};
