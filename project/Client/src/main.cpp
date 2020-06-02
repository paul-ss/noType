#include <unistd.h>

#include <filesystem>

#include "logger.hpp"
#include "game.hpp"

int main(int argc, char** argv) {
    if (chdir(std::filesystem::canonical(argv[0]).parent_path().string().c_str())) {
        return 1;
    }

    init_logger();
    BOOST_LOG_TRIVIAL(info) << "notype start";
    Game game;
    game.Run();
    BOOST_LOG_TRIVIAL(info) << "notype end";
    return 0;
}
