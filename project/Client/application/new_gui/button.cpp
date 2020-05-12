#include "baseElement.hpp"

Button::Button() {
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
}
