#pragma once
#include <Sprocket.h>
#include <glm/glm.hpp>
#include <apecs.hpp>

void add_tree(apx::registry& registry, glm::ivec2 position);

void add_rock(apx::registry& registry, glm::ivec2 position);
void add_iron(apx::registry& registry, glm::ivec2 position);
void add_tin(apx::registry& registry, glm::ivec2 position);
void add_mithril(apx::registry& registry, glm::ivec2 position);