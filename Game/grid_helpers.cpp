#include "grid_helpers.h"
#include <string>
#include <glm/glm.hpp>

namespace {

void add_rock_base(
    apx::registry& registry,
    glm::ivec2 pos,
    const std::string& material,
    const std::string& name)
{
    auto entity = registry.create();
    registry.emplace<spkt::NameComponent>(entity, name);

    auto& tr = registry.emplace<spkt::Transform3DComponent>(entity);
    tr.position = {pos.x + 0.5f, 0.0f, pos.y + 0.5f};
    tr.position.y -= spkt::Random(0.0f, 0.5f);
    tr.orientation = glm::rotate(
        glm::identity<glm::quat>(),
        glm::half_pi<float>() * spkt::Random(0, 3),
        {0.0, 1.0, 0.0}
    );
    tr.scale = {1.1f, 1.1f, 1.1f};

    auto& mc = registry.emplace<spkt::ModelComponent>(entity);
    mc.mesh = "Resources/Models/Rock.obj";
    mc.material = material;

    // Add the new entity to the grid.
    auto tile_map = registry.find<spkt::TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<spkt::TileMapSingleton>(tile_map);
    tms.tiles[pos] = entity;
}

}

void add_tree(apx::registry& registry, glm::ivec2 position)
{
    auto entity = registry.create();
    registry.emplace<spkt::NameComponent>(entity, "Tree");

    auto& tr = registry.emplace<spkt::Transform3DComponent>(entity);
    tr.position = {position.x + 0.5f, 0.0f, position.y + 0.5f};
    tr.orientation = glm::rotate(
        glm::identity<glm::quat>(),
        spkt::Random(0.0f, 360.0f),
        {0, 1, 0}
    );
    float r = spkt::Random(1.0f, 1.3f);
    tr.scale = {r, r, r};

    auto& mc = registry.emplace<spkt::ModelComponent>(entity);
    mc.mesh = "Resources/Models/BetterTree.obj";
    mc.material = "Resources/Materials/tree.yaml";

    // Add the new entity to the grid.
    auto tile_map = registry.find<spkt::TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<spkt::TileMapSingleton>(tile_map);
    tms.tiles[position] = entity;
}

void add_rock(apx::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/rock.yaml", "Rock");
}

void add_iron(apx::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/iron.yaml", "Iron");
}

void add_tin(apx::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/tin.yaml", "Tin");
}

void add_mithril(apx::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/mithril.yaml", "Mithril");
}