#include "button.hpp"

#define BUTTON_STYLE_PATH "assets/media/styles/button.json"

Button::Button(std::weak_ptr<EventManager> l_eMgr) : BaseElement(BUTTON_STYLE_PATH, l_eMgr) {
    /*
    try {
        std::shared_ptr<EventManager> eMgr(_eventMgr);
        auto lambdaClick = [this](EventDetails& l_details) {
            this->HandleClick(l_details);
        };
        eMgr->AddCallback(StateType(0), "Mouse_Left", lambdaClick);

        auto lambdaRelease = [this](EventDetails& l_details) {
            this->HandleRelease(l_details);
        };
        eMgr->AddCallback(StateType(0), "Mouse_Left_Release", lambdaRelease);

        auto lambdaTextEntered = [this](EventDetails& l_details) {
            this->HandleTextEntered(l_details);
        };
        eMgr->AddCallback(StateType(0), "Text_Entered", lambdaTextEntered);
    } catch (std::bad_weak_ptr& e) {
        //log
    }
    */
}

    void Button::ReadIn(const std::string& l_stream) {

    }
    void Button::OnClick(const sf::Vector2f& l_mousePos) {

    }
    void Button::OnRelease() {

    }
    void Button::OnHover(const sf::Vector2f& l_mousePos) {

    }
    void Button::OnLeave() {

    }
    void Button::Update(float l_dT) {

    }
    void Button::Draw(std::shared_ptr<sf::RenderTarget> l_target) {

    }