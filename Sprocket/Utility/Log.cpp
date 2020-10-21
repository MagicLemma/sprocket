#include "Log.h"

#include <exception>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Sprocket {
namespace Log {

namespace {
	std::shared_ptr<spdlog::logger> s_logger_p;
}

void Init()
{
	if (s_logger_p == nullptr) {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_logger_p = spdlog::stdout_color_mt("Sprocket");
		s_logger_p->set_level(spdlog::level::trace);
	}
}

std::shared_ptr<spdlog::logger>& Logger()
{
	return s_logger_p;
}

}
}