#include "PathCalculator.h"

#include <Sprocket/Utility/Log.h>

#include <glm/glm.hpp>

#include <deque>
#include <queue>
#include <memory>

namespace {

struct path_node
{
    glm::ivec2 position;
    float g;
    float h;
    std::shared_ptr<path_node> parent = nullptr;
};

using PathNodePtr = std::shared_ptr<path_node>;

glm::ivec2 closest_square(const glm::vec3& position)
{
    int gridX = (int)std::round(position.x - 0.5f);
    int gridZ = (int)std::round(position.z - 0.5f);
    return {gridX, gridZ};
}

PathNodePtr FindNode(
    const std::vector<PathNodePtr>& nodes,
    const glm::ivec2& coords)
{
    for (auto& node : nodes) {
        if (node->position == coords) {
            return node;
        }
    }
    return nullptr;
};

float Heuristic(PathNodePtr node, const glm::ivec2& target)
{
    auto delta = target - node->position;
    return 10.0f * std::sqrt(std::pow(delta.x, 2) + std::pow(delta.y, 2));
};

float score(path_node& node)
{
    return node.g + node.h;
}

}

std::queue<glm::vec3> make_astar_path(
    glm::vec3 start,
    glm::vec3 end,
    const grid_function& gridFunction)
{
    auto p1 = closest_square(start);
    auto p2 = closest_square(end);

    glm::ivec2 directions[8] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };

    std::vector<PathNodePtr> openList;
    std::vector<PathNodePtr> closedList;

    PathNodePtr current = nullptr;

    auto firstNode = std::make_shared<path_node>();
    firstNode->position = p1;
    firstNode->g = 0;
    firstNode->h = Heuristic(firstNode, p2); // Give this a proper value;
    openList.push_back(firstNode);

    while (!openList.empty()) {
        auto currentIt = openList.begin();
        
        for (auto it = openList.begin(); it != openList.end(); ++it) {
            if (score(**it) <= score(**currentIt)) {
                currentIt = it;
            }
        }

        current = *currentIt;
        if (current->position == p2) { break; }

        closedList.push_back(current);
        openList.erase(currentIt);

        for (const auto& dir : directions) {
            glm::ivec2 newPos = current->position + dir;

            if (FindNode(closedList, newPos) || gridFunction(newPos)) {
                continue;
            }

            float dirLength = glm::length(glm::vec2(dir));
            int totalCost = current->g + (int)(10 * dirLength);

            auto successor = FindNode(openList, newPos);
            if (successor == nullptr) {
                successor = std::make_shared<path_node>();
                successor->position = newPos;
                successor->g = totalCost;
                successor->h = Heuristic(successor, p2); // Give this a proper value;
                successor->parent = current;
                openList.push_back(successor);
            }
            else if (totalCost < successor->g) {
                successor->g = totalCost;
                successor->parent = current;
            }
        }
    }

    std::queue<glm::vec3> path;

    if (current->position != p2) {
        spkt::log::info("No path found!");
        return path;
    }

    std::vector<glm::vec3> aStarPath;

    while (current != nullptr) {
        auto p = current->position;
        aStarPath.push_back({p.x + 0.5f, end.y, p.y + 0.5f});
        current = current->parent;
    }

    for (auto it = aStarPath.rbegin(); it != aStarPath.rend(); ++it) {
        path.push(*it);
    }

    path.push(end);
    return path;
}