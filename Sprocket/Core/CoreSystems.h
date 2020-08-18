#pragma once

namespace Sprocket {

class Window;
class ModelManager;
class TextureManager;

struct CoreSystems
{
    Window*         window;
    ModelManager*   modelManager;
    TextureManager* textureManager;
};

}