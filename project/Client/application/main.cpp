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
    return 0;
}
