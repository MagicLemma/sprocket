#include "animation.h"

#include <sprocket/utility/maths.h>

namespace spkt {
namespace {

template <typename T>
decltype(auto) get_adjacent_keyframes(const std::vector<timed_attr<T>>& arr, float time)
{
    assert(arr.size() > 0);
    for (auto it = std::next(arr.begin()); it != arr.end(); ++it) {
        if (it->time > time) {
            return std::tie(*std::prev(it), *it);
        }
    }
    return std::tie(arr.front(), arr.front());
}

float get_ratio(float before, float after, float time)
{
    return (before == after) ? 0.0f : (time - before) / (after - before);
}

glm::vec3 mix_from_keyframe(const std::vector<timed_attr<glm::vec3>>& values, float time)
{
    const auto& [before, after] = get_adjacent_keyframes(values, time);
    return glm::mix(before.attr, after.attr, get_ratio(before.time, after.time, time));
}

glm::quat slerp_from_keyframe(const std::vector<timed_attr<glm::quat>>& values, float time)
{
    const auto& [before, after] = get_adjacent_keyframes(values, time);
    return glm::slerp(before.attr, after.attr, get_ratio(before.time, after.time, time));
}

glm::mat4 transform_from_keyframes(const bone_key_frames& keyframe_data, float time)
{
    glm::vec3 position = mix_from_keyframe(keyframe_data.positions, time);
    glm::quat orientation = slerp_from_keyframe(keyframe_data.orientations, time);
    glm::vec3 scale = mix_from_keyframe(keyframe_data.scales, time);
    return make_transform(position, orientation, scale);
}

void get_pose_recursive(
    std::vector<glm::mat4>& pose,
    const skeleton& skeleton,
    const animation& animation,
    float time,
    std::uint32_t bone_index,
    const glm::mat4& parent_transform)
{
    const bone& bone = skeleton.bones[bone_index];
    const auto& kfData = animation.key_frames[bone_index];

    glm::mat4 transform = parent_transform * transform_from_keyframes(kfData, time);
    pose[bone_index] = transform * bone.offset;

    for (const auto& child : bone.children) {
        get_pose_recursive(pose, skeleton, animation, time, child, transform);
    }   
}

}

std::vector<glm::mat4> skeleton::get_pose(const std::string& animation, float time) const
{
    std::vector<glm::mat4> pose;
    pose.resize(bones.size(), glm::mat4(1.0));

    if (auto it = animations.find(animation); it != animations.end()) {
        float t = modulo(time, it->second.duration);
        get_pose_recursive(pose, *this, it->second, t, 0, glm::mat4(1.0));
    }
    return pose;
}

}