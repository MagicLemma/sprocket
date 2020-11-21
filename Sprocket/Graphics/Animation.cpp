#include "Animation.h"
#include "Types.h"
#include "Adaptors.h"

namespace Sprocket {
namespace {

glm::vec3 GetPosition(const BoneKeyFrames& bkf, float time)
{
    if (bkf.keyPostitions.size() == 1) {
        return bkf.keyPostitions[0].position;
    }

    u32 before = 0, after = 0;
    for (u32 i = 0; i != bkf.keyPostitions.size(); ++i) {
        if (bkf.keyPostitions[i].time > time) {
            before = i > 0 ? i - 1 : bkf.keyPostitions.size() - 1;
            after = i;
            break;
        }
    }
    auto& beforeKF = bkf.keyPostitions[before];
    auto& afterKF = bkf.keyPostitions[after];
    float delta = (time - beforeKF.time) / (afterKF.time - beforeKF.time);
    return Maths::Interpolate(beforeKF.position, afterKF.position, delta);
}

glm::quat GetOrientation(const BoneKeyFrames& bkf, float time)
{
    if (bkf.keyOrientations.size() == 1) {
        return bkf.keyOrientations[0].orientation;
    }

    u32 before = 0, after = 0;
    for (u32 i = 0; i != bkf.keyOrientations.size(); ++i) {
        if (bkf.keyOrientations[i].time > time) {
            before = i > 0 ? i - 1 : bkf.keyOrientations.size() - 1;
            after = i;
            break;
        }
    }
    auto& beforeKF = bkf.keyOrientations[before];
    auto& afterKF = bkf.keyOrientations[after];
    float delta = (time - beforeKF.time) / (afterKF.time - beforeKF.time);
    return Maths::Interpolate(beforeKF.orientation, afterKF.orientation, delta);
}

glm::vec3 GetScale(const BoneKeyFrames& bkf, float time)
{
    if (bkf.keyScales.size() == 1) {
        return bkf.keyScales[0].scale;
    }

    u32 before = 0, after = 0;
    for (u32 i = 0; i != bkf.keyScales.size(); ++i) {
        if (bkf.keyScales[i].time > time) {
            before = i > 0 ? i - 1 : bkf.keyScales.size() - 1;
            after = i;
            break;
        }
    }
    auto& beforeKF = bkf.keyScales[before];
    auto& afterKF = bkf.keyScales[after];
    float delta = (time - beforeKF.time) / (afterKF.time - beforeKF.time);
    return Maths::Interpolate(beforeKF.scale, afterKF.scale, delta);
}

glm::mat4 GetAnimationTransform(const BoneKeyFrames& kfData, float time)
{
    glm::vec3 position = GetPosition(kfData, time);
    glm::quat orientation = GetOrientation(kfData, time);
    glm::vec3 scale = GetScale(kfData, time);
    return Maths::Transform(position, orientation, scale);
}

void GetPoseRec(
    std::vector<glm::mat4>& pose,
    const Skeleton& skeleton,
    const Animation& animation,
    float time,
    u32 boneIndex,
    const glm::mat4& parentTransform
)
{
    const Bone& bone = skeleton.bones[boneIndex];
    const auto& kfData = animation.keyFrames[boneIndex];

    glm::mat4 transform = parentTransform * GetAnimationTransform(kfData, time);
    pose[boneIndex] = transform * bone.offset;

    for (const auto& child : bone.children) {
        GetPoseRec(pose, skeleton, animation, time, child, transform);
    }   
}

}

std::vector<glm::mat4> Skeleton::GetPose(const std::string& animation, float time) const
{
    std::vector<glm::mat4> pose;
    pose.resize(bones.size());

    auto it = animations.find(animation);
    if (it != animations.end()) {
        float t = Maths::Modulo(time, it->second.duration);
        GetPoseRec(pose, *this, it->second, t, 0, glm::mat4(1.0));
    }
    else {
        for (auto& x : pose) {
            x = glm::mat4(1.0);
        }
    }
    return pose;
}

}