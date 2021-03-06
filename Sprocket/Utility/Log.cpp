#include "Log.h"

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <chrono>

#ifdef _WIN32
#include "Windows.h"
#endif

namespace Sprocket {
namespace log {
namespace {

std::chrono::system_clock::duration time_today()
{
	using days = std::chrono::duration<int, std::ratio<86400>>; // Will be in C++20

	auto now = std::chrono::system_clock::now();
	return now - std::chrono::floor<days>(now);
}

}

void init()
{
#ifdef _WIN32
	static bool inited = false;
	if (!inited) {
		DWORD termFlags;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (GetConsoleMode(handle, &termFlags))
			SetConsoleMode(handle, termFlags | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		inited = true;
	}
#endif
}

void prefixed_log(std::string_view prefix, std::string_view message)
{
#ifdef _WIN32
	static const auto once = [] () -> int {
		DWORD termFlags;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (GetConsoleMode(handle, &termFlags))
			SetConsoleMode(handle, termFlags | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		return 0;
	}();
#endif

	fmt::print(fmt::fg(fmt::color::white), "{:%H:%M:%S} [", time_today());
    fmt::print(prefix);
    fmt::print(fmt::fg(fmt::color::white), "] {}\n", message);
}

}
}