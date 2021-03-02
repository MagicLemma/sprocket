#pragma once
#include <string>
#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace log {

void init();

template <typename... Args>
void warn(const char* format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::azure), "[WARN]: {}\n", message);
}

template <typename... Args>
void info(const char* format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::light_green), "[INFO]: {}\n", message);
}

template <typename... Args>
void error(const char* format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::orange), "[ERROR]: {}\n", message);
}

template <typename... Args>
void fatal(const char* format, Args&&... args)
{
    std::string message = fmt::format(format, std::forward<Args>(args)...);
    fmt::print(fmt::fg(fmt::color::magenta), "[FATAL]: {}\n", message);
}
}
}