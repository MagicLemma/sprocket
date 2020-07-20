#pragma once
#include <type_traits>

namespace Sprocket {

float RandomFloat(float min, float max);
int RandomInt(int min, int max);

template <typename T>
T Random(T min, T max)
{
    if constexpr (std::is_same<T, int>::value) {
        return RandomInt(min, max);
    }

    else if constexpr (std::is_same<T, float>::value) {
        return RandomFloat(min, max);
    }

    else if constexpr (std::is_same<T, Maths::vec2>::value) {
        return {
            RandomFloat(min.x, max.x),
            RandomFloat(min.y, max.y)
        };
    }

    else if constexpr (std::is_same<T, Maths::vec3>::value) {
        return {
            RandomFloat(min.x, max.x),
            RandomFloat(min.y, max.y),
            RandomFloat(min.z, max.z)
        };
    }

    else if constexpr (std::is_same<T, Maths::vec4>::value) {
        return {
            RandomFloat(min.x, max.x),
            RandomFloat(min.y, max.y),
            RandomFloat(min.z, max.z),
            RandomFloat(min.w, max.w)
        };
    }

    else {
        static_assert(sizeof(T) == -1);
    }
}

}