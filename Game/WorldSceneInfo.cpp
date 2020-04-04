#include "WorldSceneInfo.h"

BasicSceneInfo::BasicSceneInfo(Sprocket::Accessor& accessor)
    : lens(accessor.window()->aspectRatio())
    , camera(&firstCamera)
    , skybox(Sprocket::Model3D("Resources/Models/Skybox.obj"),
             Sprocket::CubeMap(
                 {"Resources/Textures/Skybox/Skybox_X_Pos.png",
                 "Resources/Textures/Skybox/Skybox_X_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Z_Neg.png"}))
{
}