#pragma once
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>

namespace Sprocket {
namespace Log {

void init();

std::shared_ptr<spdlog::logger> &logger();

}
}

// Log macros
#define SPKT_LOG_TRACE(...) ::Sprocket::Log::logger()->trace(__VA_ARGS__)
#define SPKT_LOG_INFO(...)  ::Sprocket::Log::logger()->info(__VA_ARGS__)
#define SPKT_LOG_WARN(...)  ::Sprocket::Log::logger()->warn(__VA_ARGS__)
#define SPKT_LOG_ERROR(...) ::Sprocket::Log::logger()->error(__VA_ARGS__)
#define SPKT_LOG_FATAL(...) ::Sprocket::Log::logger()->critical(__VA_ARGS__)