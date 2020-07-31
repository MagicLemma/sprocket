#pragma once

namespace Sprocket {

class Window;
class SceneManager;
class ModelManager;
class TextureManager;

struct CoreSystems
{
    Window*         window;
    SceneManager*   sceneManager;
    ModelManager*   modelManager;
    TextureManager* textureManager;
};

}