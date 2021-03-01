#pragma once
#include "Maths.h"
#include "Types.h"

#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

namespace Sprocket {

struct Bone
{
    std::string name;
    u32 index;

    // A transform that only applies to this bone and does not get added
    // into the transform that is passed down to child bones.
    glm::mat4 offset;

    // The indices of the bones that are children of this one.
    std::vector<u32> children;
};

struct KeyFramePos
{
    f32         time;
    glm::vec3 position;
};

struct KeyFrameOri
{
    f32         time;
    glm::quat orientation;
};

struct KeyFrameScl
{
    f32         time;
    glm::vec3 scale;
};

struct BoneKeyFrames
{
    std::vector<KeyFramePos> keyPostitions;
    std::vector<KeyFrameOri> keyOrientations;
    std::vector<KeyFrameScl> keyScales;
};

struct Animation
{
    std::string name;

    // The duration in seconds of this animation.
    f32 duration;

    // A vector of keyFrames for each bone. This vector will be the same
    // length as the bone vector and the indices will line up.
    std::vector<BoneKeyFrames> keyFrames;
};

struct Skeleton
{
    std::vector<Bone> bones;
    std::unordered_map<std::string, u32> boneMap;
    std::unordered_map<std::string, Animation> animations;

    std::vector<glm::mat4> GetPose(const std::string& name, f32 time) const;
};

}