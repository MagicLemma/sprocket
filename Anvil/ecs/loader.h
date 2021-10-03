#pragma once
#include "ecs.h"

#include <string>
#include <functional>

namespace anvil {

using save_predicate = std::function<bool(const registry&, entity)>;

// Saves all entities satisfying the given predicate to the given file.
void save_registry_to_file(
    const std::string& file,
    const registry& reg,
    const save_predicate& predicate
);

void load_registry_from_file(const std::string& file, registry& reg);

// Creates a copy of the given Entity within the given Scene. The given
// entity can be from a different scene.
entity copy_entity(registry& scene, entity entity);

// Copies one scene into another. The target scene is first cleared.
void copy_registry(const registry& source, registry& target);

}
