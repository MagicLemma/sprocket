#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    Maths::mat4 transform;
    Maths::mat4 finalTransform;

    std::vector<uint32_t> children;
};

struct Skeleton
{
    std::vector<Bone> bones;
    std::unordered_map<std::string, std::uint32_t> boneMap;
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