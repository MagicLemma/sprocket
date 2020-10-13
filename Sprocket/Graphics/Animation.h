#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    std::string name;
    Maths::mat4 transform;
};

struct KeyFrame
{
    float time;
    std::unordered_map<std::string, Maths::mat4> transforms;
        // TODO: Potentially bad structure
};

struct Animation
{
    std::string           name;
    float                 duration;
    std::vector<KeyFrame> keyFrames;
};

}