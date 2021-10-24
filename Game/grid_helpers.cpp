#include "grid_helpers.h"

#include <Sprocket/Utility/random.h>

#include <glm/glm.hpp>

#include <string>

namespace {

void add_rock_base(
    game::registry& registry,
    glm::ivec2 pos,
    const std::string& material,
    const std::string& name)
{
    auto entity = registry.create();
    registry.emplace<game::NameComponent>(entity, name);

    auto& tr = registry.emplace<game::Transform3DComponent>(entity);
    tr.position = {pos.x + 0.5f, 0.0f, pos.y + 0.5f};
    tr.position.y -= spkt::random_from_range(0.0f, 0.5f);
    tr.orientation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::half_pi<float>() * spkt::random_from_range(0, 3),
        {0.0, 1.0, 0.0}
    );
    tr.scale = {1.1f, 1.1f, 1.1f};

    auto& mc = registry.emplace<game::StaticModelComponent>(entity);
    mc.mesh = "Resources/Models/Rock.obj";
    mc.material = material;

    // Add the new entity to the grid.
    auto tile_map = registry.find<game::TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<game::TileMapSingleton>(tile_map);
    tms.tiles[pos] = entity;
}

}

void add_tree(game::registry& registry, glm::ivec2 position)
{
    auto entity = registry.create();
    registry.emplace<game::NameComponent>(entity, "Tree");

    auto& tr = registry.emplace<game::Transform3DComponent>(entity);
    tr.position = {position.x + 0.5f, 0.0f, position.y + 0.5f};
    tr.orientation = glm::rotate(
        glm::identity<glm::quat>(),
        spkt::random_angle_degrees(),
        {0, 1, 0}
    );
    float r = spkt::random_from_range(1.0f, 1.3f);
    tr.scale = {r, r, r};

    auto& mc = registry.emplace<game::StaticModelComponent>(entity);
    mc.mesh = "Resources/Models/BetterTree.obj";
    mc.material = "Resources/Materials/tree.yaml";

    // Add the new entity to the grid.
    auto tile_map = registry.find<game::TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<game::TileMapSingleton>(tile_map);
    tms.tiles[position] = entity;
}

void add_rock(game::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/rock.yaml", "Rock");
}

void add_iron(game::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/iron.yaml", "Iron");
}

void add_tin(game::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/tin.yaml", "Tin");
}

void add_mithril(game::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/mithril.yaml", "Mithril");
}