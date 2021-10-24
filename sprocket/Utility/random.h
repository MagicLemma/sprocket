#pragma once
#include <numbers>

namespace spkt {

int random_from_range(int min, int max);
float random_from_range(float min, float max);

inline float random_uniform() { return random_from_range(0.0f, 1.0f); }
inline float random_half_angle() { return random_from_range(0.0f, std::numbers::pi_v<float>); }
inline float random_angle() { return random_from_range(0.0f, 2.0f * std::numbers::pi_v<float>); }
inline float random_half_angle_degrees() { return random_from_range(0.0f, 180.0f); }
inline float random_angle_degrees() { return random_from_range(0.0f, 360.0f); }

}