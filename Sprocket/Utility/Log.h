#pragma once
#include <string>
#include <memory>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace log {

void init();

template <typename... Args>
void warn(std::string_view format, Args&&... args)
{
    std::string prefix = fmt::format(fmt::fg(fmt::color::aqua), "WARN");
    std::string log = fmt::format(format, std::forward<Args>(args)...);
    prefixed_log(prefix, log);
}

template <typename... Args>
void info(std::string_view format, Args&&... args)
{
    std::string prefix = fmt::format(fmt::fg(fmt::color::light_green), "INFO");
    std::string log = fmt::format(format, std::forward<Args>(args)...);
    prefixed_log(prefix, log);
}

template <typename... Args>
void error(std::string_view format, Args&&... args)
{
    std::string prefix = fmt::format(fmt::fg(fmt::color::orange), "ERROR");
    std::string log = fmt::format(format, std::forward<Args>(args)...);
    prefixed_log(prefix, log);
}

template <typename... Args>
void fatal(std::string_view format, Args&&... args)
{
    std::string prefix = fmt::format(fmt::fg(fmt::color::magenta), "FATAL");
    std::string log = fmt::format(format, std::forward<Args>(args)...);
    prefixed_log(prefix, log);
}

void prefixed_log(std::string_view prefix, std::string_view message);

}
}