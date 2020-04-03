#include "WorldSceneInfo.h"

BasicSceneInfo::BasicSceneInfo(Sprocket::Accessor& accessor)
    : lens(accessor.window()->aspectRatio())
    , camera(&firstCamera)
    , skybox(accessor.resourceManager()->loadModel3D("Resources/Models/Skybox.obj"),
             accessor.resourceManager()->loadCubeMap(
                {"Resources/Textures/Skybox/Skybox_X_Pos.png",
                 "Resources/Textures/Skybox/Skybox_X_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                 "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                 "Resources/Textures/Skybox/Skybox_Z_Neg.png"}))
{
}