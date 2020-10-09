#pragma once

namespace Sprocket {

class Window;
class AssetManager;
class TextureManager;
class MaterialManager;

struct CoreSystems
{
    Window*          window;
    AssetManager*    assetManager;
    TextureManager*  textureManager;
    MaterialManager* materialManager;
};

}