#pragma once
#include <string>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace log {

void init();

template <typename... Args>
void warn(const char* format, Args&&... args)
{
    fmt::print(fmt::fg(fmt::color::azure), format, std::forward<Args>(args)...);
    fmt::print("\n");
}

template <typename... Args>
void info(const char* format, Args&&... args)
{
    fmt::print(fmt::fg(fmt::color::light_green), format, std::forward<Args>(args)...);
    fmt::print("\n");
}

template <typename... Args>
void error(const char* format, Args&&... args)
{
    fmt::print(fmt::fg(fmt::color::orange), format, std::forward<Args>(args)...);
    fmt::print("\n");
}

template <typename... Args>
void fatal(const char* format, Args&&... args)
{
    fmt::print(fmt::fg(fmt::color::magenta), format, std::forward<Args>(args)...);
    fmt::print("\n");
}

}
}