#include "Entry.h"

#include <iostream>

#include <spdlog/spdlog.h>

namespace Sprocket {

int begin(int argc, char** argv)
{
    std::cout << "In begin\n" << std::flush;
    spdlog::warn("Library log {}", 100);
    return 5;
}

}