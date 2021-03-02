#pragma once
#include <string>
#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace log {

void init();

template <typename Format, typename... Args>
void warn(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::azure), "[WARN]: {}\n", message);
}

template <typename Format, typename... Args>
void info(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::light_green), "[INFO]: {}\n", message);
}

template <typename Format, typename... Args>
void error(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::orange), "[ERROR]: {}\n", message);
}

template <typename Format, typename... Args>
void fatal(Format& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::magenta), "[FATAL]: {}\n", message);
}

}
}