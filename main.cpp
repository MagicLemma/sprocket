#include <iostream>

#include <spdlog/spdlog.h>
#include <library.h>

int main(int argc, char* argv[])
{
    spdlog::warn("Format my log message {} {}", "WITH THINGS", 234);
    std::cout << foosticles(42) << std::endl;
    return 0;
}