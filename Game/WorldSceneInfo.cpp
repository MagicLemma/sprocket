#include "WorldSceneInfo.h"

BasicSceneInfo::BasicSceneInfo(Sprocket::Window* window)
    : window(window)
    , lens(window->aspectRatio())
    , skybox({"Resources/Textures/Skybox/Skybox_X_Pos.png",
                "Resources/Textures/Skybox/Skybox_X_Neg.png",
                "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                "Resources/Textures/Skybox/Skybox_Z_Neg.png"})
{}