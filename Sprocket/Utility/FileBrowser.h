#pragma once
#include <string>

namespace spkt {

class window;

std::string OpenFile(window* window, const char* filter);
std::string SaveFile(window* window, const char* filter);

}