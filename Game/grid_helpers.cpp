#include "grid_helpers.h"

namespace {

void add_rock_base(
    spkt::registry& registry,
    glm::ivec2 pos,
    std::string_view material,
    std::string_view name)
{
    using namespace spkt;

    auto newEntity = apx::create_from(registry);
    auto& n = newEntity.emplace<NameComponent>();
    n.name = name;

    auto& tr = newEntity.emplace<Transform3DComponent>();
    tr.position = {pos.x + 0.5f, 0.0f, pos.y + 0.5f};
    tr.position.y -= Random(0.0f, 0.5f);
    float randomRotation = glm::half_pi<float>() * Random(0, 3);
    tr.orientation = glm::rotate(glm::identity<glm::quat>(), randomRotation, {0.0, 1.0, 0.0});
    tr.scale = {1.1f, 1.1f, 1.1f};

    auto& modelData = newEntity.emplace<ModelComponent>();
    modelData.mesh = "Resources/Models/Rock.obj";
    modelData.material = material;
    newEntity.emplace<SelectComponent>();

    // Add the new entity to the grid.
    auto tile_map = registry.find<TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<TileMapSingleton>(tile_map);
    tms.tiles[pos] = newEntity.entity();
}

}

void add_tree(spkt::registry& registry, glm::ivec2 position)
{
    auto newEntity = apx::create_from(registry);

    auto& name = newEntity.emplace<spkt::NameComponent>();
    name.name = "Tree";

    auto& tr = newEntity.emplace<spkt::Transform3DComponent>();
    tr.position = {position.x + 0.5f, 0.0f, position.y + 0.5f};
    tr.orientation = glm::rotate(
        glm::identity<glm::quat>(),
        spkt::Random(0.0f, 360.0f),
        {0, 1, 0}
    );
    float r = spkt::Random(1.0f, 1.3f);
    tr.scale = {r, r, r};

    auto& modelData = newEntity.emplace<spkt::ModelComponent>();
    modelData.mesh = "Resources/Models/BetterTree.obj";
    modelData.material = "Resources/Materials/tree.yaml";
    newEntity.emplace<spkt::SelectComponent>();

    // Add the new entity to the grid.
    auto tile_map = registry.find<spkt::TileMapSingleton>();
    assert(registry.valid(tile_map));
    auto& tms = registry.get<spkt::TileMapSingleton>(tile_map);
    tms.tiles[position] = newEntity.entity();
}

void add_rock(spkt::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/rock.yaml", "Rock");
}

void add_iron(spkt::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/iron.yaml", "Iron");
}

void add_tin(spkt::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/tin.yaml", "Tin");
}

void add_mithril(spkt::registry& registry, glm::ivec2 position)
{
    add_rock_base(registry, position, "Resources/Materials/mithril.yaml", "Mithril");
}