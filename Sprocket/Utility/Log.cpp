#include "Log.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>

namespace Sprocket {
namespace Log {
namespace {

#ifdef _WIN32
HANDLE g_stdin;
DWORD  g_old_mode;
#endif

}

void Init()
{
#ifdef _WIN32
	g_stdin = GetStdHandle(STD_INPUT_HANDLE);
	if (g_stdin == INVALID_HANDLE_VALUE) {
		std::cerr << "Could not get stdin handle\n";
		return;
	}

	//if (!GetConsoleMode(g_stdin, &g_old_mode)) {
	//	std::cerr << "Could not get current console mode\n";
	//	return;
	//}
//
	//DWORD mode = g_old_mode | ENABLE_VIRTUAL_TERMINAL_INPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	//if (!SetConsoleMode(g_stdin, mode)) {
	//	std::cerr << "Failed to set console colouring\n";
	//	return;
	//}

	for (int k = 1; k < 255; ++k)
	{
		// pick the colorattribute k you want
		SetConsoleTextAttribute(g_stdin, k);
		std::cout << k << " I want to be nice today!" << std::endl;
	}
#endif
}

}
}