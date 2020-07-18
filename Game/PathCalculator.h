#pragma once
#include "GameGrid.h"

#include <Sprocket.h>

#include <vector>

std::vector<Sprocket::Maths::vec3> GeneratePath(
    const Sprocket::Maths::vec3& start,
    const Sprocket::Maths::vec3& end,
    const GameGrid& grid
);

std::pair<int, int> ClosestGridSquare(
    const Sprocket::Maths::vec3& position,
    const GameGrid& grid
);
