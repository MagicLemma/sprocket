#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    std::string name;

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
    float       time;
    Maths::vec3 position;
    Maths::quat orientation;
};

struct Animation
{
    std::string name;
    float       duration;
    std::vector<std::vector<KeyFrame>> boneKeyFrames;
        // The outer vector is a map from bone index to that bones
        // key frames. Bones may not have key frames at the same time
        // as other bones. Thus, when interpolating, we do a separate
        // interpolation for each bone. Another way to think of it:
        // rather than a key frame containing an array of bone data,
        // we have an array of bones containing key frame data.
};

}