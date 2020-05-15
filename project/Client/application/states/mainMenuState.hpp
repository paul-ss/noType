#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "button.hpp"

class MainMenuState : public BaseState {
public:
    explicit MainMenuState(std::weak_ptr<StateManager> stateManager);
    ~MainMenuState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

    void MouseClick(EventDetails& l_details);
    void MouseRelease(EventDetails& l_details);
    void Play(EventDetails& l_details);
    void Quit(EventDetails& l_details);
    void TextEntered(EventDetails& l_details);

private:
    std::vector<std::shared_ptr<BaseElement>> _elements;
    //sf::Font m_font;
    //sf::Text m_text;
//
    //sf::Vector2f m_buttonSize;
    //sf::Vector2f m_buttonPos;
    //unsigned int m_buttonPadding;
//
    //sf::RectangleShape m_rects[3];
    //sf::Text m_labels[3];
};
