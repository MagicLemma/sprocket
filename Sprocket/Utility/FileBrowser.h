#pragma once
#include "Window.h"

#include <string>

namespace Sprocket {

std::string OpenFile(Window* window, const char* filter);
std::string SaveFile(Window* window, const char* filter);

}