#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace spkt {

struct Bone
{
    std::string name;
    std::uint32_t index;

    // A transform that only applies to this bone and does not get added
    // into the transform that is passed down to child bones.
    glm::mat4 offset;

    // The indices of the bones that are children of this one.
    std::vector<std::uint32_t> children;
};

struct KeyFramePos
{
    float     time;
    glm::vec3 position;
};

struct KeyFrameOri
{
    float     time;
    glm::quat orientation;
};

struct KeyFrameScl
{
    float     time;
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
    float duration;

    // A vector of keyFrames for each bone. This vector will be the same
    // length as the bone vector and the indices will line up.
    std::vector<BoneKeyFrames> keyFrames;
};

struct Skeleton
{
    std::vector<Bone> bones;
    std::unordered_map<std::string, std::uint32_t> boneMap;
    std::unordered_map<std::string, Animation> animations;

    std::vector<glm::mat4> GetPose(const std::string& name, float time) const;
};

}