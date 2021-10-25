#include "random.h"

#include <random>
#include <numbers>

namespace spkt {

int random_from_range(int min, int max)
{
    static std::default_random_engine gen;
    return std::uniform_int_distribution(min, max)(gen);
}

float random_from_range(float min, float max)
{
    static std::default_random_engine gen;
    return std::uniform_real_distribution(min, max)(gen);
}
    
}