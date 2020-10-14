#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    std::string name;
    Maths::mat4 transform;
    std::vector<uint32_t> children;
        // All the data for the bone, this should not change after loading.

    Maths::mat4 finalTransform;
        // When setting the pose of the model, this stores the final
        // transform which will get uploaded to the shader. As such, it will
        // change quite a lot.

};

struct Skeleton
{
    std::vector<Bone> bones;
    std::unordered_map<std::string, std::uint32_t> boneMap;
};

struct KeyFramePos
{
    float       time;
    Maths::vec3 position;
};

struct KeyFrameOri
{
    float       time;
    Maths::quat orientation;
};

struct Animation
{
    std::string name;
    float       duration;
    std::vector<std::vector<KeyFramePos>> boneKeyFramesPos;
    std::vector<std::vector<KeyFrameOri>> boneKeyFramesOri;
        // The outer vector is a map from bone index to that bones
        // key frames. Bones may not have key frames at the same time
        // as other bones. Thus, when interpolating, we do a separate
        // interpolation for each bone. Another way to think of it:
        // rather than a key frame containing an array of bone data,
        // we have an array of bones containing key frame data.
};

}