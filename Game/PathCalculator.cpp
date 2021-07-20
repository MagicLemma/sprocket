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
    path_node* parent = nullptr;
};

glm::ivec2 closest_square(const glm::vec3& position)
{
    int gridX = (int)std::round(position.x - 0.5f);
    int gridZ = (int)std::round(position.z - 0.5f);
    return {gridX, gridZ};
}

path_node* find_node(
    const std::vector<std::unique_ptr<path_node>>& nodes,
    const glm::ivec2& coords)
{
    for (auto& node : nodes) {
        if (node->position == coords) {
            return node.get();
        }
    }
    return nullptr;
};

float score(path_node& node)
{
    return node.g + node.h;
}

std::unique_ptr<path_node> pop_node_lowest_score(std::vector<std::unique_ptr<path_node>>& nodes)
{
    std::unique_ptr<path_node> ret_val = nullptr;

    if (nodes.empty()) { return ret_val; }

    auto current = nodes.begin();
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        if (score(**it) < score(**current)) {
            current = it;
        }
    }

    ret_val = std::move(*current);
    nodes.erase(current);
    return ret_val;
}

float heuristic(glm::ivec2 pos, glm::ivec2 target)
{
    auto delta = target - pos;
    return 10.0f * std::sqrtf(std::powf(delta.x, 2) + std::powf(delta.y, 2));
};

}

std::queue<glm::vec3> make_astar_path(
    glm::vec3 start, glm::vec3 end, const grid_function& gridFunction
) {
    auto p1 = closest_square(start);
    auto p2 = closest_square(end);

    glm::ivec2 directions[8] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };

    std::vector<std::unique_ptr<path_node>> openList;
    std::vector<std::unique_ptr<path_node>> closedList;

    auto firstNode = std::make_unique<path_node>();
    firstNode->position = p1;
    firstNode->g = 0;
    firstNode->h = heuristic(p1, p2); // Give this a proper value;
    openList.push_back(std::move(firstNode));

    std::unique_ptr<path_node> current = nullptr;
    path_node* curr = nullptr;
    while (current = pop_node_lowest_score(openList)) {
        curr = current.get();
        if (current->position == p2) { break; }

        for (const auto& dir : directions) {
            glm::ivec2 newPos = current->position + dir;

            if (find_node(closedList, newPos) || gridFunction(newPos)) {
                continue;
            }

            float dirLength = glm::length(glm::vec2(dir));
            float totalCost = current->g + (10.0f * dirLength);

            auto successor = find_node(openList, newPos);
            if (successor == nullptr) {
                auto new_node = std::make_unique<path_node>();
                new_node->position = newPos;
                new_node->g = totalCost;
                new_node->h = heuristic(newPos, p2); // Give this a proper value;
                new_node->parent = current.get();
                openList.push_back(std::move(new_node));
            }
            else if (totalCost < successor->g) {
                successor->g = totalCost;
                successor->parent = current.get();
            }
        }

        closedList.push_back(std::move(current));
    }

    std::queue<glm::vec3> path;

    if (current->position != p2) {
        spkt::log::info("No path found!");
        return path;
    }

    std::vector<glm::vec3> aStarPath;

    while (curr != nullptr) {
        auto p = curr->position;
        aStarPath.push_back({p.x + 0.5f, end.y, p.y + 0.5f});
        curr = curr->parent;
    }

    for (auto it = aStarPath.rbegin(); it != aStarPath.rend(); ++it) {
        path.push(*it);
    }

    path.push(end);
    return path;
}