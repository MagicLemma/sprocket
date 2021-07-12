#pragma once
#include "ecs.h"

#include <string>

namespace spkt {

void save_registry_to_file(const std::string& file, spkt::registry* reg);
void load_registry_from_file(const std::string& file, spkt::registry* reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
spkt::entity copy_entity(spkt::registry* scene, spkt::entity entity);

// Copies one scene into another. The target scene is first cleared.
void copy_registry(spkt::registry* source, spkt::registry* target);

}