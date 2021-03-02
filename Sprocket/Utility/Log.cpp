#include "Log.h"

#include <fmt/core.h>
#include <fmt/color.h>

#ifdef _WIN32
#include "Windows.h"
#endif

namespace Sprocket {
namespace log {
namespace {

bool g_inited = false;

}

void init()
{
#ifdef _WIN32
	if (!g_inited) {
		DWORD termFlags;
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (GetConsoleMode(handle, &termFlags))
			SetConsoleMode(handle, termFlags | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
		g_inited = true;
	}
#endif
}

}
}