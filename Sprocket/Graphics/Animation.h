#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    std::string name;
    std::uint32_t index; // The index of this bone within the bone vector.
    Maths::mat4 transform;

    std::vector<uint32_t> children;
        // All the data for the bone, this should not change after loading.

    Maths::mat4 finalTransform;
        // When setting the pose of the model, this stores the final
        // transform which will get uploaded to the shader. As such, it will
        // change quite a lot.

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

struct BoneKeyFrames
{
    std::vector<KeyFramePos> keyPostitions;
    std::vector<KeyFrameOri> keyOrientations;
};

struct Animation
{
    std::string name;
    float       duration;
    std::vector<BoneKeyFrames> keyFrames;
        // A vector of keyFrames for each bone. This vector will be the same
        // length as the bone vector and the indices will line up.
};

struct Skeleton
{
    std::vector<Bone> bones;
    std::unordered_map<std::string, std::uint32_t> boneMap;
    std::unordered_map<std::string, Animation> animations;
};

}