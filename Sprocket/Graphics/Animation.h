#pragma once
#include "Maths.h"

#include <string>

namespace Sprocket {

struct Bone
{
    std::string name;
    std::uint32_t index; // The index of this bone within the bone vector.

    Maths::mat4 transform;
    Maths::mat4 offset;

    std::vector<uint32_t> children;
        // All the data for the bone, this should not change after loading.

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

struct KeyFrameScl
{
    float       time;
    Maths::vec3 scale;
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