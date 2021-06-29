#include "Random.h"

#include <random>

namespace spkt {

float RandomFloat(float min, float max)
{
    static std::mt19937 gen;
    return std::uniform_real_distribution(min, max)(gen);
}

int RandomInt(int min, int max)
{
    static std::mt19937 gen;
    return std::uniform_int_distribution(min, max)(gen);
}
    
    
}