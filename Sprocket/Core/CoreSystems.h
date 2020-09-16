#pragma once

namespace Sprocket {

class Window;
class ModelManager;
class TextureManager;
class MaterialManager;

struct CoreSystems
{
    Window*          window;
    ModelManager*    modelManager;
    TextureManager*  textureManager;
    MaterialManager* materialManager;
};

}