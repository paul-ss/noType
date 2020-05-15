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
//
    //for (boost::property_tree::ptree::value_type& guiEvent : root) {
    //    std::cout << typeid(static_cast<std::string>(guiEvent.first.data())).name() << "\n";
    //}

    return 0;
}
