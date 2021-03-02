#include "Log.h"

#include <fmt/core.h>
#include <fmt/color.h>

#ifdef _WIN32
#include "Windows.h"
#endif

namespace Sprocket {
namespace log {

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

}
}