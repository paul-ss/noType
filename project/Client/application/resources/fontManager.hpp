#pragma once

#include <SFML/Graphics/Font.hpp>

#include "resourceManager.hpp"

#define FONTS_FILE_PATH "assets/fonts.json"

class FontManager : public ResourceManager<FontManager, sf::Font> {
public:
    FontManager() : ResourceManager(FONTS_FILE_PATH, "fonts") {}

    std::shared_ptr<sf::Font> load(const std::string& l_path) {
        auto font = std::make_shared<sf::Font>();
        if (!font->loadFromFile(std::filesystem::absolute(l_path))) {
            font.reset();
            font = nullptr;
            BOOST_LOG_TRIVIAL(error) << "Failed to load font file: " << l_path;
        }
        return font;
    }
};
