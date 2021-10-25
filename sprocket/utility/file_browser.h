#pragma once
#include <optional>
#include <string>

namespace spkt {

class window;

std::optional<std::string> open_file(window* window, const char* filter);
std::optional<std::string> save_file(window* window, const char* filter);

}