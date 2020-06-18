#include "ShadowBox.h"
#include "Log.h"

namespace Sprocket {

ShadowBox::ShadowBox(const StaticCamera* lightView,
                     const Camera* camera,
                     const PerspectiveLens* lens,
                     const Window* window)
    : d_lightView(lightView)
    , d_camera(camera)
    , d_lens(lens)
    , d_window(window)
    , d_farWidth(SHADOW_DISTANCE * std::tan(Maths::Radians(lens->FOV())))
    , d_nearWidth(lens->NearPlane() * std::tan(Maths::Radians(lens->FOV())))
    , d_farHeight(d_farWidth / window->AspectRatio())
    , d_nearHeight(d_nearWidth / window->AspectRatio())
{
}

Maths::vec4 ShadowBox::CalculateLightSpaceFrustumCorner(
    const Maths::vec3& start,
    const Maths::vec3& direction,
    float width,
    bool debug
)
{
    if (debug) {
        SPKT_LOG_INFO("width = {}", width);
    }
    Maths::vec3 point = start + width * direction;
    Maths::vec4 point4f = {point.x, point.y, point.z, 1.0f};
    return d_lightView->View() * point4f;
}

std::array<Maths::vec4, 8> ShadowBox::CalculateFrustumVertices(
    const Maths::mat4& rotation,
    const Maths::vec3& forward,
    const Maths::vec3& centreNear,
    const Maths::vec3& centreFar
)
{
    Maths::vec3 upVec = rotation * UP;
    Maths::Normalise(upVec);
    Maths::vec3 rightVec = Maths::Cross(forward, upVec);
    Maths::Normalise(rightVec);
    Maths::vec3 downVec = -upVec;
    Maths::vec3 leftVec = -rightVec;

    Maths::vec3 farTop = centreFar + d_farHeight * upVec;
    Maths::vec3 farBot = centreFar + d_farHeight * downVec;
    Maths::vec3 nearTop = centreNear + d_nearHeight * upVec;
    Maths::vec3 nearBot = centreNear + d_nearHeight * downVec;

    std::array<Maths::vec4, 8> points;
    points[0] = CalculateLightSpaceFrustumCorner(farTop, rightVec, d_farWidth, true);
    points[1] = CalculateLightSpaceFrustumCorner(farTop, leftVec, d_farWidth);
    points[2] = CalculateLightSpaceFrustumCorner(farBot, rightVec, d_farWidth);
    points[3] = CalculateLightSpaceFrustumCorner(farBot, leftVec, d_farWidth);
  
    points[4] = CalculateLightSpaceFrustumCorner(nearTop, rightVec, d_nearWidth);
    points[5] = CalculateLightSpaceFrustumCorner(nearTop, leftVec, d_nearWidth);
    points[6] = CalculateLightSpaceFrustumCorner(nearBot, rightVec, d_nearWidth);
    points[7] = CalculateLightSpaceFrustumCorner(nearBot, leftVec, d_nearWidth);
    
    SPKT_LOG_INFO("point 0 = {},{},{}", points[0].x, points[0].y, points[0].z);
    return points;
}

void ShadowBox::OnUpdate(const StaticCamera& lightView)
{
    Maths::quat rotationQuat = Maths::identity;
    rotationQuat = Maths::Rotate(rotationQuat, {0, 1, 0}, 0);
    rotationQuat = Maths::Rotate(rotationQuat, {1, 0, 0}, 0);
    Maths::mat4 rotation = Maths::ToMat4(rotationQuat);

    Maths::vec3 forward = rotation * FORWARD;
    Maths::Normalise(forward);
    
    Maths::vec3 toFar = SHADOW_DISTANCE * forward;
    Maths::vec3 toNear = 0.1f * forward;

    Maths::vec3 centreFar = toFar + d_camera->Position();
    Maths::vec3 centreNear = toNear + d_camera->Position();

    auto points = CalculateFrustumVertices(rotation, forward, centreNear, centreFar);

    bool first = true;
    for (const auto& point : points) {
        if (first) {
            d_minX = point.x;
            d_maxX = point.x;
            d_minY = point.y;
            d_maxY = point.y;
            d_minZ = point.z;
            d_maxZ = point.z;
            first = false;
            continue;
        }
        d_maxX = std::max(point.x, d_maxX);
        d_minX = std::min(point.x, d_minX);

        d_maxY = std::max(point.y, d_maxY);
        d_minY = std::min(point.y, d_minY);

        d_maxZ = std::max(point.z, d_maxZ);
        d_minZ = std::min(point.z, d_minZ);
    }
    d_maxZ += OFFSET;
}

Maths::vec3 ShadowBox::GetCentre() const
{
    float x = (d_minX + d_maxX) / 2.0f;
    float y = (d_minY + d_maxY) / 2.0f;
    float z = (d_minZ + d_maxZ) / 2.0f;
    Maths::vec4 centre = {x, y, z, 1.0};
    Maths::mat4 invertedLight = Maths::Inverse(d_lightView->View());
    return invertedLight * centre;
}

float ShadowBox::GetWidth() const
{
    return d_maxX - d_minX;
}

float ShadowBox::GetHeight() const
{
    return d_maxY - d_minY;
}

float ShadowBox::GetLength() const
{
    return d_maxZ - d_minZ;
}

}