#pragma once
#include "Maths.h"
#include "Camera.h"
#include "StaticCamera.h"
#include "Window.h"
#include "PerspectiveLens.h"

#include <array>

namespace Sprocket {

class ShadowBox
// A class that represents a box that contains the view frustum of
// a projection matrix. Used for calculating shadow maps.
{
    const float OFFSET = 0.0f;
    const Maths::vec4 UP = {0.0, 1.0, 0.0, 0.0};
    const Maths::vec4 FORWARD = {0.0, 0.0, -1.0, 0.0};
    const float SHADOW_DISTANCE = 100.0f;

    float d_minX = 0;
    float d_maxX = 0;
    float d_minY = 0;
    float d_maxY = 0;
    float d_minZ = 0;
    float d_maxZ = 0;

    const StaticCamera*    d_lightView;

    const Camera*          d_camera; // Player camera
    const PerspectiveLens* d_lens; // Player lens

    const Window*          d_window;

    float d_farWidth, d_nearWidth, d_farHeight, d_nearHeight;

    Maths::vec4 CalculateLightSpaceFrustumCorner(
        const Maths::vec3& start,
        const Maths::vec3& direction,
        float width,
        bool debug = false
    );

    std::array<Maths::vec4, 8> CalculateFrustumVertices(
        const Maths::mat4& rotation,
        const Maths::vec3& forward,
        const Maths::vec3& centreNear,
        const Maths::vec3& centreFar
    );

public:
    ShadowBox(const StaticCamera* lightView,
              const Camera* camera,
              const PerspectiveLens* lens,
              const Window* window);

    void OnUpdate(
        const StaticCamera& lightView
    );

    Maths::vec3 GetCentre() const;
    float GetWidth() const;
    float GetHeight() const;
    float GetLength() const;
};

}