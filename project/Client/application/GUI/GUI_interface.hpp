#pragma once
#include "GUI_element.hpp"

#include <unordered_map>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

using Elements = std::unordered_map<std::string,GUI_Element*>;

class GUI_Manager;

class GUI_Interface : public GUI_Element{
    friend class GUI_Element;
    friend class GUI_Manager;
public:
    explicit GUI_Interface(const std::string& name, GUI_Manager* guiManager);
    ~GUI_Interface();

    void SetPosition(const sf::Vector2f& pos);

    bool AddElement(const GUI_ElementType& type, const std::string& name);
    GUI_Element* GetElement(const std::string& name) const;
    bool RemoveElement(const std::string& name);

    bool HasParent() const;
    GUI_Manager* GetManager() const;

    bool IsInside(const sf::Vector2f& point) const;

    void Focus();
    void Defocus();
    bool IsFocused();

    void ReadIn(std::stringstream& stream);
    void OnClick(const sf::Vector2f& mousePos);
    void OnRelease();
    void OnHover(const sf::Vector2f& mousePos);
    void OnLeave();
    void OnTextEntered(const char& letter);

    bool IsBeingMoved() const;
    bool IsMovable() const;
    void BeginMoving();
    void StopMoving();

    void ApplyStyle();

    const sf::Vector2f& GetPadding() const;
    void SetPadding(const sf::Vector2f& padding);
    sf::Vector2f GetGlobalPosition() const;
    const sf::Vector2f& GetContentSize() const;

    void Redraw();
    bool NeedsContentRedraw() const;
    void RedrawContent();
    bool NeedsControlRedraw() const;
    void RedrawControls();

    void ToggleTitleBar();

    void Update(float dT);
    void Draw(sf::RenderTarget* target);

    void UpdateScrollHorizontal(unsigned int percent);
    void UpdateScrollVertical(unsigned int percent);
private:
    void DefocusTextfields();
    Elements _elements;
    sf::Vector2f _elementPadding;

    GUI_Interface* _parent;
    GUI_Manager* _guiManager;

    sf::RenderTexture* _backdropTexture;
    sf::Sprite _backdrop;

    // Movement.
    sf::RectangleShape _titleBar;
    sf::Vector2f _moveMouseLast;
    bool _showTitleBar;
    bool _movable;
    bool _beingMoved;
    bool _focused;

    // Variable size.
    void AdjustContentSize(const GUI_Element* reference = nullptr);
    void SetContentSize(const sf::Vector2f& vec);
    sf::RenderTexture* _contentTexture;
    sf::Sprite _content;
    sf::Vector2f _contentSize;
    int _scrollHorizontal;
    int _scrollVertical;
    bool _contentRedraw;

    // Control layer.
    sf::RenderTexture* _controlTexture;
    sf::Sprite _control;
    bool _controlRedraw;
};
