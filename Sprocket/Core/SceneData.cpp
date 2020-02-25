#include "Core/SceneData.h"

namespace Sprocket {
    
SceneData::SceneData(const std::string& name, SceneType type, Window* window)
    : d_name(name)
    , d_type(type)
    , d_window(window)
    , d_paused(false)
{

}

}