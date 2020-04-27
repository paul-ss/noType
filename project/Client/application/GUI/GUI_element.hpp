#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <unordered_map>

#include "GUI_style.hpp"
#include <sstream>

enum class GUI_ElementType{ None = -1, Window, Label, Button, Textfield };

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;

class GUI_Element {
    friend class GUI_Interface;

    public:
        GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner);
        virtual ~GUI_Element();

        virtual void ReadIn(std::stringstream& stream) = 0;
        virtual void OnClick(const sf::Vector2f& mousePos) = 0;
        virtual void OnRelease() = 0;
        virtual void OnHover(const sf::Vector2f& mousePos) = 0;
        virtual void OnLeave() = 0;
        virtual void Update(float dT) = 0;
        virtual void Draw(sf::RenderTarget* target) = 0;

        virtual void UpdateStyle(const GUI_ElementState& state, const GUI_Style& style);
        virtual void ApplyStyle();

        GUI_ElementType GetType();

        const std::string& GetName() const;
        void SetName(const std::string& name);
        const sf::Vector2f& GetPosition() const;
        void SetPosition(const sf::Vector2f& pos);
        const sf::Vector2f& GetSize() const;
        GUI_ElementState GetState() const;
        void SetState(const GUI_ElementState& state);
        void SetRedraw(const bool& redraw);
        bool NeedsRedraw() const;
        void SetOwner(GUI_Interface* owner);
        GUI_Interface* GetOwner() const;
        bool HasOwner() const;
        bool IsActive() const;
        void SetActive(const bool& active);
        bool IsInside(const sf::Vector2f& point) const;
        sf::Vector2f GetGlobalPosition() const;
        bool IsControl() const;
        std::string GetText() const;
        void SetText(const std::string& text);

        friend std::stringstream& operator >>(
                std::stringstream& stream, GUI_Element& b)
        {
            b.ReadIn(stream);
            return stream;
        }

    protected:
        void applyTextStyle();
        void applyBgStyle();
        void applyGlyphStyle();

        void requireTexture(const std::string& name);
        void requireFont(const std::string& name);
        void releaseTexture(const std::string& name);
        void releaseFont(const std::string& name);
        void releaseResources();

    protected:
        std::string _name;
        sf::Vector2f _position;
        ElementStyles _style;
        GUI_Visual _visual;
        GUI_ElementType _type;
        GUI_ElementState _state;
        GUI_Interface* _owner;

        bool _needsRedraw;
        bool _active;
        bool _isControl;
};
