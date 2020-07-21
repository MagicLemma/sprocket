#pragma once
#include <Sprocket.h>

#include <queue>
#include <memory>
#include <functional>

using GridFunction = std::function<bool(const Sprocket::Maths::ivec2&)>;
    // A function used to query the grid. Accepts coordinates and returns
    // true if that location in the grid is filled.

struct PathNode
{
    Sprocket::Maths::ivec2 position;
    int g;
    int h;
    std::shared_ptr<PathNode> parent = nullptr;

    int Score() const { return g + h; }
};

using PathNodePtr = std::shared_ptr<PathNode>;

PathNodePtr FindNode(
    const std::vector<PathNodePtr>& nodes,
    const Sprocket::Maths::ivec2& coords
);

float Heuristic(
    PathNodePtr node,
    const Sprocket::Maths::ivec2& target
);

std::queue<Sprocket::Maths::vec3> GenerateAStarPath(
    const Sprocket::Maths::vec3& start,
    const Sprocket::Maths::vec3& end,
    GridFunction gridFunction
);

Sprocket::Maths::ivec2 ClosestGridSquare(
    const Sprocket::Maths::vec3& position
);
