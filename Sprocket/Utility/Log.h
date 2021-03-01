#pragma once
#include <string_view>
#include <fmt/core.h>
#include <fmt/color.h>

namespace Sprocket {
namespace Log {

void Init();

template <typename Format, typename... Args>
void Info(Format&& format, Args&&... args)
{
    fmt::print(fmt::fg(fmt::color::yellow), format, std::forward<Args>(args)...);
    fmt::print("\n");
}

}
}

// Log macros
#define SPKT_LOG_TRACE(...) ::Sprocket::Log::Info(__VA_ARGS__);
#define SPKT_LOG_INFO(...)  ::Sprocket::Log::Info(__VA_ARGS__);
#define SPKT_LOG_WARN(...)  ::Sprocket::Log::Info(__VA_ARGS__);
#define SPKT_LOG_ERROR(...) ::Sprocket::Log::Info(__VA_ARGS__);
#define SPKT_LOG_FATAL(...) ::Sprocket::Log::Info(__VA_ARGS__);