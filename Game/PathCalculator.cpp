#include "PathCalculator.h"

#include <Sprocket/Core/log.h>

#include <glm/glm.hpp>

#include <deque>
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

float heuristic(glm::vec2 pos, glm::vec2 target)
{
    return 10.0f * glm::distance(pos, target);
};

std::unique_ptr<path_node> make_path_node(glm::ivec2 pos, glm::ivec2 dest, float cost, path_node* parent = nullptr)
{
    auto new_node = std::make_unique<path_node>();
    new_node->position = pos;
    new_node->g = cost;
    new_node->h = heuristic(pos, dest);
    new_node->parent = parent;
    return new_node;
}

}

std::deque<glm::vec3> make_astar_path(
    glm::vec3 start, glm::vec3 end, const grid_function_t& grid_function
) {
    auto p1 = closest_square(start);
    auto p2 = closest_square(end);

    glm::ivec2 directions[8] = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},
        {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };

    std::vector<std::unique_ptr<path_node>> open_list;
    std::vector<std::unique_ptr<path_node>> closed_list;

    open_list.push_back(make_path_node(p1, p2, 0));

    std::unique_ptr<path_node> current = nullptr;
    path_node* curr = nullptr;
    while (current = pop_node_lowest_score(open_list)) {
        curr = current.get();
        if (curr->position == p2) { break; }

        for (const auto& dir : directions) {
            glm::ivec2 new_pos = curr->position + dir;

            if (find_node(closed_list, new_pos) || grid_function(new_pos)) {
                continue;
            }

            float cost = curr->g + 10.0f * glm::length(glm::vec2(dir));

            auto successor = find_node(open_list, new_pos);
            if (successor == nullptr) {
                open_list.push_back(make_path_node(new_pos, p2, cost, curr));
            }
            else if (cost < successor->g) {
                successor->g = cost;
                successor->parent = curr;
            }
        }

        closed_list.push_back(std::move(current));
    }

    std::deque<glm::vec3> path;

    if (curr->position != p2) {
        spkt::log::info("No path found!");
        return path;
    }

    path.push_back(end);
    while (curr != nullptr) {
        auto p = curr->position;
        path.push_front({p.x + 0.5f, end.y, p.y + 0.5f});
        curr = curr->parent;
    }

    return path;
}