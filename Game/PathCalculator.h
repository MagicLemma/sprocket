#pragma once
#include <queue>
#include <memory>
#include <functional>
#include <vector>

#include <glm/glm.hpp>

using GridFunction = std::function<bool(const glm::ivec2&)>;
    // A function used to query the grid. Accepts coordinates and returns
    // true if that location in the grid is filled.

struct PathNode
{
    glm::ivec2 position;
    int g;
    int h;
    std::shared_ptr<PathNode> parent = nullptr;

    int Score() const { return g + h; }
};

using PathNodePtr = std::shared_ptr<PathNode>;

PathNodePtr FindNode(
    const std::vector<PathNodePtr>& nodes,
    const glm::ivec2& coords
);

float Heuristic(
    PathNodePtr node,
    const glm::ivec2& target
);

std::queue<glm::vec3> GenerateAStarPath(
    const glm::vec3& start,
    const glm::vec3& end,
    GridFunction gridFunction
);

glm::ivec2 ClosestGridSquare(
    const glm::vec3& position
);
