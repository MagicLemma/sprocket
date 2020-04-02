#include "WorldSceneInfo.h"

BasicSceneInfo::BasicSceneInfo(Sprocket::Accessor& accessor)
    : lens(accessor.window()->aspectRatio())
    , camera(&firstCamera)
    , skybox({"Resources/Textures/Skybox/Skybox_X_Pos.png",
                "Resources/Textures/Skybox/Skybox_X_Neg.png",
                "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                "Resources/Textures/Skybox/Skybox_Z_Neg.png"})
{
    accessor.loadModel3D("Plane", "Resources/Models/Plane.obj");
    accessor.loadModel3D("Deagle", "Resources/Models/Deagle.obj");
    accessor.loadModel3D("Cube", "Resources/Models/Cube.obj");
    accessor.loadModel3D("Dragon", "Resources/Models/Dragon.obj");
}