#include "Log.h"

#include <exception>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Sprocket {
namespace Log {

namespace {
	std::shared_ptr<spdlog::logger> s_coreLogger_p;
	std::shared_ptr<spdlog::logger> s_clientLogger_p;
}

void init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_coreLogger_p = spdlog::stdout_color_mt("Sprocket");
	s_coreLogger_p->set_level(spdlog::level::trace);

	s_clientLogger_p = spdlog::stdout_color_mt("Application");
	s_clientLogger_p->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& coreLogger()
{
	return s_coreLogger_p;
}

std::shared_ptr<spdlog::logger>& clientLogger()
{
	return s_clientLogger_p;
}

}
}