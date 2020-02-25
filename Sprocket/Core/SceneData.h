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

class SceneData
{
    // Core data
    std::string d_name;
    SceneType   d_type;
    Window*     d_window;

    // Extra attributes
    bool        d_paused;

public:
    SceneData(const std::string& name, SceneType type, Window* window);

    // Core data getters
    std::string name() const { return d_name;}
    SceneType type() const { return d_type; }
    Window* window() const { return d_window; }

    // Extra attributes are modifiable
    bool paused() const { return d_paused; }
    void paused(bool value) { d_paused = value; }

};

}