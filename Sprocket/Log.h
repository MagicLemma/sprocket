#pragma once
#include <memory>
#include <iostream>

#include <spdlog/spdlog.h>

namespace Sprocket {
namespace Log {

void init();

std::shared_ptr<spdlog::logger> &coreLogger();
std::shared_ptr<spdlog::logger> &clientLogger();

}
}

// Core log macros
#define SPKT_LOG_CORE_TRACE(...) ::Sprocket::Log::coreLogger()->trace(__VA_ARGS__)
#define SPKT_LOG_CORE_INFO(...)  ::Sprocket::Log::coreLogger()->info(__VA_ARGS__)
#define SPKT_LOG_CORE_WARN(...)  ::Sprocket::Log::coreLogger()->warn(__VA_ARGS__)
#define SPKT_LOG_CORE_ERROR(...) ::Sprocket::Log::coreLogger()->error(__VA_ARGS__)
#define SPKT_LOG_CORE_FATAL(...) ::Sprocket::Log::coreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SPKT_LOG_TRACE(...)      ::Sprocket::Log::clientLogger()->trace(__VA_ARGS__)
#define SPKT_LOG_INFO(...)       ::Sprocket::Log::clientLogger()->info(__VA_ARGS__)
#define SPKT_LOG_WARN(...)       ::Sprocket::Log::clientLogger()->warn(__VA_ARGS__)
#define SPKT_LOG_ERROR(...)      ::Sprocket::Log::clientLogger()->error(__VA_ARGS__)
#define SPKT_LOG_FATAL(...)      ::Sprocket::Log::clientLogger()->critical(__VA_ARGS__)