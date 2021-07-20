#pragma once
#include <glm/glm.hpp>

#include <functional>
#include <deque>

using grid_function_t = std::function<bool(const glm::ivec2&)>;
    // A function used to query the grid. Accepts coordinates and returns
    // true if that location in the grid is filled.

std::deque<glm::vec3> make_astar_path(
    glm::vec3 start, glm::vec3 end, const grid_function_t& gridFunction
);
