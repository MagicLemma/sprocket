#pragma once
#include "Model3D.h"

#include <string>

namespace Sprocket {

class Window;
class SceneManager;

struct CoreSystems
{
    Window*       window;
    SceneManager* sceneManager;
};

}