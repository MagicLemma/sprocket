#pragma once

namespace Sprocket {

class Window;
class SceneManager;
class ModelManager;

struct CoreSystems
{
    Window*       window;
    SceneManager* sceneManager;
    ModelManager* modelManager;
};

}