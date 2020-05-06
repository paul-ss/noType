#pragma once

#include <SFML/Graphics/Font.hpp>

#include "resourceManager.hpp"

class FontManager : public ResourceManager<FontManager, sf::Font> {
    public:
        FontManager() : ResourceManager("assets/fonts.cfg") {}

        sf::Font* load(const std::string& path) {
            sf::Font* font = new sf::Font();
            if (!font->loadFromFile(std::filesystem::absolute(path))) {
                delete font;
                font = nullptr;
                BOOST_LOG_TRIVIAL(error) << "Failed to load font file: " << path;
            }
            return font;
        }
};
