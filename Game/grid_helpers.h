#pragma once
#include <Sprocket.h>
#include <glm/glm.hpp>

void add_tree(spkt::registry& registry, glm::ivec2 position);

void add_rock(spkt::registry& registry, glm::ivec2 position);
void add_iron(spkt::registry& registry, glm::ivec2 position);
void add_tin(spkt::registry& registry, glm::ivec2 position);
void add_mithril(spkt::registry& registry, glm::ivec2 position);