#pragma once
#include <string>

namespace spkt {

class Window;

std::string OpenFile(Window* window, const char* filter);
std::string SaveFile(Window* window, const char* filter);

}