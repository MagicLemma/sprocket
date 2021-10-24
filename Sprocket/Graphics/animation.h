#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace spkt {

struct bone
{
    std::string name;
    std::uint32_t index;

    // A transform that only applies to this bone and does not get added
    // into the transform that is passed down to child bones.
    glm::mat4 offset;

    // The indices of the bones that are children of this one.
    std::vector<std::uint32_t> children;
};

template <typename T>
struct timed_attr
{
    float time;
    T     attr;
};

struct bone_key_frames
{
    std::vector<spkt::timed_attr<glm::vec3>> positions;
    std::vector<spkt::timed_attr<glm::quat>> orientations;
    std::vector<spkt::timed_attr<glm::vec3>> scales;
};

struct animation
{
    std::string name;

    // The duration in seconds of this animation.
    float duration;

    // A vector of keyFrames for each bone. This vector will be the same
    // length as the bone vector and the indices will line up.
    std::vector<bone_key_frames> key_frames;
};

struct skeleton
{
    std::vector<bone> bones;
    std::unordered_map<std::string, std::uint32_t> bone_map;
    std::unordered_map<std::string, animation> animations;

    std::vector<glm::mat4> get_pose(const std::string& name, float time) const;
};

}