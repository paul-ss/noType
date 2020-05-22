#include "baseElement.hpp"
#include "sharedContext.hpp"

#define PATH_TO_STYLES "assets/media/styles/"

BaseElement::BaseElement(const ElementName l_name,
        std::weak_ptr<SharedContext> l_sharedContext,
        const sf::Vector2f& l_position,
        const std::string& l_style) :
        _name(l_name),
        _position(l_position),
        _state(ElementState::Neutral),
        _sharedContext(l_sharedContext) {

    loadStyle(l_style);
        }

void BaseElement::applyStyle(const std::shared_ptr<Style>& l_style) {
    try {
        std::shared_ptr<SharedContext>sharedContext(_sharedContext);

        // Background
        if (!l_style->backgroundImage.empty()) {
            std::shared_ptr<TextureManager>tMgr(sharedContext->textureManager);
            if (tMgr->RequireResource(l_style->backgroundImage)) {
                std::shared_ptr<sf::Texture>tempTexture(tMgr->GetResource(l_style->backgroundImage));
                _visual.backgroundImage.setTexture(*tempTexture);
                _visual.backgroundImage.setColor(l_style->backgroundImageColor);
                sf::FloatRect rectImage = _visual.backgroundImage.getLocalBounds();
                _visual.backgroundImage.setOrigin(rectImage.left + rectImage.width / 2.0f,
                        rectImage.top + rectImage.height / 2.0f);
                _visual.backgroundImage.setPosition(_position);
            }
        }

        _visual.backgroundSolid.setFillColor(l_style->backgroundColor);
        _visual.backgroundSolid.setSize(l_style->size);
        sf::FloatRect rectSolid = _visual.backgroundSolid.getLocalBounds();
        _visual.backgroundSolid.setOrigin(rectSolid.left + rectSolid.width / 2.0f,
                rectSolid.top + rectSolid.height / 2.0f);
        _visual.backgroundSolid.setPosition(_position);

        // Text
        if (!l_style->textFont.empty()) {
            std::shared_ptr<FontManager>fMgr(sharedContext->fontManager);
            if (fMgr->RequireResource(l_style->textFont)) {
                std::shared_ptr<sf::Font>tempFont(fMgr->GetResource(l_style->textFont));
                _visual.text.setFont(*tempFont);
                _visual.text.setFillColor(l_style->textColor);
                _visual.text.setCharacterSize(l_style->textSize);
                if (l_style->textCenterOrigin) {
                    sf::FloatRect rect = _visual.text.getLocalBounds();
                    _visual.text.setOrigin(rect.left + rect.width / 2.0f,
                        rect.top + rect.height / 2.0f);
                } else {
                    _visual.text.setOrigin(0.f, 0.f);
                }
                _visual.text.setPosition(_position + l_style->textPadding);
            }
        }

        // Glyph
        if (!l_style->glyph.empty()) {
            std::shared_ptr<TextureManager>tMgr(sharedContext->textureManager);
            std::shared_ptr<sf::Texture>tempTexture(tMgr->GetResource(l_style->glyph));
            _visual.glyph.setTexture(*tempTexture);
            _visual.glyph.setPosition(_position + l_style->glyphPadding);
        }

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << e.what() << " [applyStyle - baseElement]";
    }
}

static ElementState stringToType(const std::string& l_string) {
    if (l_string == "Neutral") {
        return ElementState::Neutral;
    }
    if (l_string == "Clicked") {
        return ElementState::Clicked;
    }
    if (l_string == "Hover") {
        return ElementState::Hover;
    }
    if (l_string == "Release") {
        return ElementState::Release;
    }
    if (l_string == "TextEntered") {
        return ElementState::TextEntered;
    }
    return ElementState::Neutral;
    BOOST_LOG_TRIVIAL(error) << " State not recognized [stringToType]";
}

void BaseElement::loadStyle(const std::string& l_path) {
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(PATH_TO_STYLES + l_path, root);

        for (boost::property_tree::ptree::value_type& currState : root) {
            auto currStyle = std::make_shared<Style>();

            currStyle->isFullScreen = root.get<bool>(currState.first + ".fullScreen");
            if (!currStyle->isFullScreen) {
                std::vector<int> sizeVec;
                for (boost::property_tree::ptree::value_type& size : root.get_child(currState.first + ".size")) {
                    sizeVec.push_back(size.second.get_value<int>());
                }
                currStyle->size.x = sizeVec[0];
                currStyle->size.y = sizeVec[1];

            } else {
                std::shared_ptr<SharedContext>sharedContext(_sharedContext);
                std::shared_ptr<Window>window(sharedContext->window);
                std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());
                auto screenSize = renderWindow->getSize();
                currStyle->size.x = screenSize.x;
                currStyle->size.y = screenSize.y;
            }

            // Background
            std::vector<int> bgColorVec;
            for (boost::property_tree::ptree::value_type& bgColor : root.get_child(currState.first + ".bgColor")) {
                bgColorVec.push_back(bgColor.second.get_value<int>());
            }
            currStyle->backgroundColor.r = bgColorVec[0];
            currStyle->backgroundColor.g = bgColorVec[1];
            currStyle->backgroundColor.b = bgColorVec[2];
            currStyle->backgroundColor.a = bgColorVec[3];

            currStyle->backgroundImage = root.get<std::string>(currState.first + ".bgImage");

            std::vector<int> bgImgColorVec;
            for (boost::property_tree::ptree::value_type& bgImgColor : root.get_child(currState.first + ".bgImgColor")) {
                bgImgColorVec.push_back(bgImgColor.second.get_value<int>());
            }
            currStyle->backgroundImageColor.r = bgImgColorVec[0];
            currStyle->backgroundImageColor.g = bgImgColorVec[1];
            currStyle->backgroundImageColor.b = bgImgColorVec[2];
            currStyle->backgroundImageColor.a = bgImgColorVec[3];

            // Text
            std::vector<int> textColorVec;
            for (boost::property_tree::ptree::value_type& textColor : root.get_child(currState.first + ".textColor")) {
                textColorVec.push_back(textColor.second.get_value<int>());
            }
            currStyle->textColor.r = textColorVec[0];
            currStyle->textColor.g = textColorVec[1];
            currStyle->textColor.b = textColorVec[2];
            currStyle->textColor.a = textColorVec[3];

            currStyle->textFont = root.get<std::string>(currState.first + ".textFont");

            std::vector<int> textPaddingVec;
            for (boost::property_tree::ptree::value_type& textPadding : root.get_child(currState.first + ".textPadding")) {
                textPaddingVec.push_back(textPadding.second.get_value<int>());
            }
            currStyle->textPadding.x = textPaddingVec[0];
            currStyle->textPadding.y = textPaddingVec[1];

            currStyle->textSize = root.get<int>(currState.first + ".textSize");

            currStyle->textCenterOrigin = root.get<bool>(currState.first + ".textOriginCenter");

            // Glyph
            currStyle->glyph = root.get<std::string>(currState.first + ".glyph");

            std::vector<int> glyphPaddingVec;
            for (boost::property_tree::ptree::value_type& glyphPadding : root.get_child(currState.first + ".glyphPadding")) {
                glyphPaddingVec.push_back(glyphPadding.second.get_value<int>());
            }
            currStyle->glyphPadding.x = glyphPaddingVec[0];
            currStyle->glyphPadding.y = glyphPaddingVec[1];
            _style.emplace(stringToType(currState.first), currStyle);
        }

    } catch (const boost::property_tree::ptree_error& e) {
        BOOST_LOG_TRIVIAL(error) << e.what() << " not valid json file: " << l_path;
    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << e.what();
    }
}

ElementState BaseElement::GetState() const {
    return _state;
}

void BaseElement::SetText(const std::string& l_text) {
    _visual.text.setString(l_text);
}

void BaseElement::SetPosition(const sf::Vector2f& l_position) {
    _position.x = l_position.x;
    _position.y = l_position.y;
}

sf::Vector2f BaseElement::GetSize() const {
    return _visual.backgroundSolid.getSize();
}

sf::Vector2f BaseElement::GetPosition() const {
    return _position;
}
