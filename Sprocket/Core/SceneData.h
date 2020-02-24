#pragma once
#include "Core/Window.h"
#include <string>

namespace Sprocket {

enum class SceneType
{
    MAIN_MENU = 0, // Main menu for the game.
    MENU = 1,      // Generic menu type.
    STAGE = 2      // Generic 3D world type.
};

struct SceneData
{
    std::string name;
    SceneType   type;
    Window*     window;
    bool        paused;
};

}