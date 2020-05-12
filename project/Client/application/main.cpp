#include <iostream>
#include <filesystem>

#include "logger.hpp"
#include "game.hpp"

#include <SFML/Graphics.hpp>
//
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>

int main() {
    init_logger();
    BOOST_LOG_TRIVIAL(info) << "notype start";
    Game game;
    game.Run();
    BOOST_LOG_TRIVIAL(info) << "notype end";
//    try {
//        boost::property_tree::ptree root;
//        boost::property_tree::read_json("assets/media/styles/button.json", root);
//        std::vector<std::string> jsonPath = {
//                "button.neutral",
//                "button.hover",
//                "button.clicked"
//        };
//        //for (auto& path : jsonPath) {
//            for (boost::property_tree::ptree::value_type& currStyle : root.get_child("button.neutral.bgColor")) {
//                //sf::Color backgroundColor = currStyle.get<uint8_t>("bgColor", backgroundColor);
//
//                std::cout << currStyle.second.data() << "\n";
//
//                //sf::Color elementColor = currStyle.second;
//                //std::string backgroundImage = currStyle.get();
//                //sf::Color backgroundImageColor = currStyle.get();
////
//                //sf::Color textColor = currStyle.get();
//                //std::string textFont = currStyle.get();
//                //sf::Vector2f textPadding = currStyle.get();
//                //unsigned int textSize = currStyle.get();
//                //bool textCenterOrigin = currStyle.get();
////
//                //std::string glyph = currStyle.get();
//                //sf::Vector2f glyphPadding = currStyle.get();
//            }
//        //}
//
//    } catch (const boost::property_tree::ptree_error& e) {
//    //BOOST_LOG_TRIVIAL(error) << e.what() << " " << l_path;
//    }
//
//
    return 0;
}
