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

void Button::readIn(const std::string& l_stream) {

}
void Button::onClick(const sf::Vector2f& l_mousePos) {

}
void Button::onRelease() {

}
void Button::onHover(const sf::Vector2f& l_mousePos) {

}
void Button::onLeave() {

}
void Button::Update(float l_dT) {

}
void Button::Draw(std::shared_ptr<sf::RenderTarget> l_target) {

}

void Button::loadStyle(const std::string& l_path) {
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(l_path, root);

        std::vector<std::string> states = {
            "Neutral",
            "Clicked",
            "Hover",
        };

        for (boost::property_tree::ptree::value_type& currState : root) {
            auto currStyle = std::make_shared<GUI_Style>();

            std::vector<int> sizeVec;
            for (boost::property_tree::ptree::value_type &size : root.get_child(currState.first.data() + ".size")) {
                sizeVec.push_back(size.second.get_value<int>());
            }
            currStyle->size.x = sizeVec[0];
            currStyle->size.y = sizeVec[1];

            std::vector<int> bgColorVec;
            for (boost::property_tree::ptree::value_type &bgColor : root.get_child(static_cast<std::string>(currState.first.data()) + ".bgColor")) {
                bgColorVec.push_back(bgColor.second.get_value<int>());
            }
            currStyle->backgroundColor.r = bgColorVec[0];
            currStyle->backgroundColor.g = bgColorVec[1];
            currStyle->backgroundColor.b = bgColorVec[2];
            currStyle->backgroundColor.a = bgColorVec[3];

            std::vector<int> textColorVec;
            for (boost::property_tree::ptree::value_type &textColor : root.get_child(currState + ".textColor")) {
                textColorVec.push_back(textColor.second.get_value<int>());
            }
            currStyle->textColor.r = textColorVec[0];
            currStyle->textColor.g = textColorVec[1];
            currStyle->textColor.b = textColorVec[2];
            currStyle->textColor.a = textColorVec[3];

            std::vector<int> textPaddingVec;
            for (boost::property_tree::ptree::value_type &textPadding : root.get_child(currState + ".bgPadding")) {
                textColorVec.push_back(textPadding.second.get_value<int>());
            }
            currStyle->textPadding.x = textPaddingVec[0];
            currStyle->textPadding.y = textPaddingVec[1];

            currStyle->textCenterOrigin = root.get<bool>(currState + ".textOriginCenter");
            currStyle->textFont = root.get<std::string>(currState + ".font");

            _style.emplace(stoi(currState), currStyle);
        }

    } catch (const boost::property_tree::ptree_error& e) {
    BOOST_LOG_TRIVIAL(error) << e.what() << " not valid json file: " << l_path;
    }
}
