#pragma once
#include <string>
#include <fmt/core.h>
#include <fmt/color.h>

namespace spkt {
namespace log {

void _prefixed_log(std::string_view prefix, std::string_view message);

template <typename... Args>
void info(std::string_view format, Args&&... args)
{
    const std::string prefix = fmt::format(fmt::fg(fmt::color::light_green), "INFO");
    const std::string log = fmt::format(format, std::forward<Args>(args)...);
    log::_prefixed_log(prefix, log);
}

template <typename... Args>
void warn(std::string_view format, Args&&... args)
{
    const std::string prefix = fmt::format(fmt::fg(fmt::color::aqua), "WARN");
    const std::string log = fmt::format(format, std::forward<Args>(args)...);
    log::_prefixed_log(prefix, log);
}

template <typename... Args>
void error(std::string_view format, Args&&... args)
{
    const std::string prefix = fmt::format(fmt::fg(fmt::color::orange), "ERROR");
    const std::string log = fmt::format(format, std::forward<Args>(args)...);
    log::_prefixed_log(prefix, log);
}

template <typename... Args>
void fatal(std::string_view format, Args&&... args)
{
    const std::string prefix = fmt::format(fmt::fg(fmt::color::magenta), "FATAL");
    const std::string log = fmt::format(format, std::forward<Args>(args)...);
    log::_prefixed_log(prefix, log);
}

}
}