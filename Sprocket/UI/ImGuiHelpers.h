#pragma once
#include "Window.h"

#include <string>

namespace Sprocket {
namespace ImGuiXtra {

void File(const std::string& name,
          Window* window,
          std::string* file,
          const char* filter);

}
}