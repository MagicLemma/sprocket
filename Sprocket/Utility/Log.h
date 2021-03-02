#pragma once
#include <string>
#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace log {

constexpr fmt::color BASE = fmt::color::ghost_white;

void init();

template <typename Format, typename... Args>
void warn(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(BASE), "[");
    fmt::print(fmt::fg(fmt::color::aqua), "WARN");
    fmt::print(fmt::fg(BASE), "]: {}\n", message);
}

template <typename Format, typename... Args>
void info(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(BASE), "[");
    fmt::print(fmt::fg(fmt::color::light_green), "INFO");
    fmt::print(fmt::fg(BASE), "]: {}\n", message);
}

template <typename Format, typename... Args>
void error(Format&& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(BASE), "[");
    fmt::print(fmt::fg(fmt::color::orange), "ERROR");
    fmt::print(fmt::fg(BASE), "]: {}\n", message);
}

template <typename Format, typename... Args>
void fatal(Format& format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(BASE), "[");
    fmt::print(fmt::fg(fmt::color::magenta), "FATAL");
    fmt::print(fmt::fg(BASE), "]: {}\n", message);
}

}
}