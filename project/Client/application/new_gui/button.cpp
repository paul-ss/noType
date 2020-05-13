#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "button.hpp"
#include "logger.hpp"

#define BUTTON_STYLE_PATH "assets/media/styles/button.json"

Button::Button(std::weak_ptr<SharedContext> l_sharedContext) :
        BaseElement(BUTTON_STYLE_PATH, l_sharedContext) {
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

void Button::loadStyle(const std::string& l_path) {
    /*try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(l_path, root);

        std::vector<std::string> states = {
            "neutral",
            "clicked",
            "hover",
        };

        for (auto& currState : states) {
            std::vector<int> sizeVec;
            for (boost::property_tree::ptree::value_type &size : root.get_child_optional(currState + ".size")) {
                sizeVec.push_back(size.second.get_value_optional<int>());
            }

            std::vector<int> bgColorVec;
            for (boost::property_tree::ptree::value_type &bgColor : root.get_child_optional(currState + ".bgColor")) {
                bgColorVec.push_back();
            }

            std::vector<int> elementColorVec;
            for (boost::property_tree::ptree::value_type &bgColor : root.get_child_optional(currState + ".bgColor")) {
                bgColorVec.push_back();
            }
        }

    } catch (const boost::property_tree::ptree_error& e) {
    BOOST_LOG_TRIVIAL(error) << e.what() << " not valid json file: " << l_path;
    }*/
}