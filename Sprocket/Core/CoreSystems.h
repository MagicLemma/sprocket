#pragma once

namespace Sprocket {

class Window;
class AssetManager;
class ModelManager;
class TextureManager;
class MaterialManager;

struct CoreSystems
{
    Window*          window;
    AssetManager*    assetManager;
    ModelManager*    modelManager;
    TextureManager*  textureManager;
    MaterialManager* materialManager;
};

}