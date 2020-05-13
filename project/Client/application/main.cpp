#include "logger.hpp"
#include "game.hpp"

//#include <SFML/Graphics.hpp>
//#include <vector>
//#include <iostream>
//#include <filesystem>
//
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>

int main() {
    init_logger();
    BOOST_LOG_TRIVIAL(info) << "notype start";
    Game game;
    game.Run();
    BOOST_LOG_TRIVIAL(info) << "notype end";

        //boost::property_tree::ptree root;
        //boost::property_tree::read_json("assets/media/styles/button.json", root);
        //std::vector<std::string> jsonPath = {
        //        "button.neutral",
        //        "button.hover",
        //        "button.clicked"
        //};
//
        //for (auto& path : jsonPath) {
        //    std::vector<int> sizeVec;
        //    std::vector<uint8_t> bgColor;
        //    std::vector<uint8_t> paddingVec;
        //    //for (boost::property_tree::ptree::value_type& currStyle : root.get_child(path)) {
        //        for (boost::property_tree::ptree::value_type& size :
        //                root.get_child(path + ".size")) {
        //            std::cout << size.second.get_value_optional<int>();
        //            //sizeVec.push_back(size.second.get_value<uint8_t>());
        //        }
        //        for(boost::property_tree::ptree::value_type& color :
        //                root.get_child(path + ".bgColor")) {
        //            //bgColor.push_back(color.second.get_value<uint8_t>());
        //        }
        //        for (boost::property_tree::ptree::value_type& padding :
        //                root.get_child(path + ".textPadding")) {
        //            //paddingVec.push_back(padding.second.get_value<uint8_t>());
        //        }
        //}
    return 0;
}
