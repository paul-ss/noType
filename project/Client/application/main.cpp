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

//    boost::property_tree::ptree root;
//    boost::property_tree::read_json("assets/media/styles/button.json", root);
//
//    std::vector<std::string> states = {
//        "neutral",
//        "clicked",
//        "release",
//        "hover",
//        "leave"
//    };
//
//    for (size_t i = 0; i < states.size(); ++i) {
//        if (boost::optional<boost::property_tree::ptree&> currState =
//                root.get_child_optional(states[i]); !currState) {
//
//            states.erase(std::remove(states.begin(), states.end(), states[i]), states.end());
//        }
//    }
//
//    for (auto& currState : states) {
//        std::vector<int> sizeVec;
//        for (boost::property_tree::ptree::value_type &size : root.get_child(currState + ".size")) {
//            sizeVec.push_back(size.second.get_value<int>());
//        }
//    }
    return 0;
}
