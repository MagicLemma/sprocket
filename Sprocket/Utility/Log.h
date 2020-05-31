#pragma once
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>

namespace Sprocket {
namespace Log {

void Init();

std::shared_ptr<spdlog::logger> &Logger();

}
}

// Log macros
#define SPKT_LOG_TRACE(...) ::Sprocket::Log::Logger()->trace(__VA_ARGS__)
#define SPKT_LOG_INFO(...)  ::Sprocket::Log::Logger()->info(__VA_ARGS__)
#define SPKT_LOG_WARN(...)  ::Sprocket::Log::Logger()->warn(__VA_ARGS__)
#define SPKT_LOG_ERROR(...) ::Sprocket::Log::Logger()->error(__VA_ARGS__)
#define SPKT_LOG_FATAL(...) ::Sprocket::Log::Logger()->critical(__VA_ARGS__)