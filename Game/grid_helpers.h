#pragma once
#include <Game/ecs/ecs.h>

#include <glm/glm.hpp>

void add_tree(game::registry& registry, glm::ivec2 position);

void add_rock(game::registry& registry, glm::ivec2 position);
void add_iron(game::registry& registry, glm::ivec2 position);
void add_tin(game::registry& registry, glm::ivec2 position);
void add_mithril(game::registry& registry, glm::ivec2 position);