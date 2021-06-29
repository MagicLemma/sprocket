#pragma once
// This uses the Windows API under the hood, so these functions are only available
// on Windows. Other platforms should be implemented when needed.

#ifdef _WIN32

#include "Window.h"
#include <string>

namespace spkt {

std::string OpenFile(Window* window, const char* filter);
std::string SaveFile(Window* window, const char* filter);

}

#endif