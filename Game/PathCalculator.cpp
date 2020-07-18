#include "PathCalculator.h"

std::vector<Sprocket::Maths::vec3> GeneratePath(
    const Sprocket::Maths::vec3& start,
    const Sprocket::Maths::vec3& end,
    const GameGrid& grid)
{
    using namespace Sprocket;

    auto p1 = InitialGridSquare(start, end);
    Maths::vec3 first{p1.first + 0.5f, start.y, p1.second + 0.5f};

    auto p2 = ClosestGridSquare(end, grid);
    Maths::vec3 last{p2.first + 0.5f, end.y, p2.second + 0.5f};

    std::vector<Sprocket::Maths::vec3> path;

    if (Maths::Distance(start, end) < 1.0f) {
        path.push_back(end);
        return path;
    }

    path.push_back(start);
    path.push_back(first);

    while (p1 != p2) {
        if (p1.first > p2.first) {
            p1.first--;
        } else if (p1.first < p2.first) {
            p1.first++;
        }

        if (p1.second > p2.second) {
            p1.second--;
        } else if (p1.second < p2.second) {
            p1.second++;
        }

        Maths::vec3 pos{p1.first + 0.5f, start.y, p1.second + 0.5f};
        if (Maths::Distance(pos, end) < 1.0f) {
            path.push_back(end);
            return path;
        }
        path.push_back(pos);
    }

    path.push_back(last);
    path.push_back(end);
    return path;
}

std::pair<int, int> ClosestGridSquare(
    const Sprocket::Maths::vec3& position,
    const GameGrid& grid)
{
    using namespace Sprocket;

    int gridX = std::floor(position.x);
    int gridZ = std::floor(position.z);

    //auto DistanceTo = [](const Maths::vec3& pos, int x, int z) {
    //    Maths::vec3 target{x, pos.y, z};
    //};

    return {gridX, gridZ};
}

std::pair<int, int> InitialGridSquare(
    const Sprocket::Maths::vec3& start,
    const Sprocket::Maths::vec3& end)
{
    using namespace Sprocket;

    int gridX = std::floor(start.x);
    int gridZ = std::floor(start.z);

    int offsetX = 0;
    int offsetZ = 0;

    auto DistanceTo = [](const Maths::vec3& pos, int x, int z) {
        Maths::vec3 target{x + 0.5f, pos.y, z + 0.5f};
        return Maths::Distance(pos, target);
    };

    float distance = DistanceTo(end, gridX, gridZ);
    float minDistance = distance;

    distance = DistanceTo(end, gridX + 1, gridZ);
    if (distance < minDistance) {
        minDistance = distance;
        offsetX = 1;
        offsetZ = 0;
    }

    distance = DistanceTo(end, gridX, gridZ + 1);
    if (distance < minDistance) {
        minDistance = distance;
        offsetX = 0;
        offsetZ = 1;
    }

    distance = DistanceTo(end, gridX + 1, gridZ + 1);
    if (distance < minDistance) {
        minDistance = distance;
        offsetX = 1;
        offsetZ = 1;
    }

    return {gridX + offsetX, gridZ + offsetZ};
}