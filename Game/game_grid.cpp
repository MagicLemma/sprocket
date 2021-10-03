#include "game_grid.h"

#include <Game/ecs/ecs.h>
#include <Game/ecs/scene.h>

#include <Sprocket/Core/input_codes.h>
#include <Sprocket/Graphics/camera.h>
#include <Sprocket/Utility/input_store.h>
#include <Sprocket/Utility/Maths.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cassert>

namespace {

constexpr const char* GRID_SQUARE = "Resources/Models/Square.obj";

template <typename T>
T& get_singleton(game::registry& reg)
{
    return reg.get<T>(reg.find<T>());
}

}

void game_grid_system_init(game::registry& registry)
{
    auto singleton = registry.create();
    registry.emplace<game::Runtime>(singleton);
    registry.emplace<game::CameraSingleton>(singleton);
    auto& grid = registry.emplace<game::GameGridSingleton>(singleton);

    grid.hovered_square_entity = registry.create();
    registry.emplace<game::Runtime>(grid.hovered_square_entity);
    registry.emplace<game::NameComponent>(grid.hovered_square_entity, "Hovered Grid Highlighter");
    registry.emplace<game::StaticModelComponent>(grid.hovered_square_entity, GRID_SQUARE);
    auto& tr1 = registry.emplace<game::Transform3DComponent>(grid.hovered_square_entity);
    tr1.scale = {0.3f, 0.3f, 0.3f};

    grid.clicked_square_entity = registry.create();
    registry.emplace<game::Runtime>(grid.clicked_square_entity);
    registry.emplace<game::NameComponent>(grid.clicked_square_entity, "Clicked Grid Highlighter");
    registry.emplace<game::StaticModelComponent>(grid.clicked_square_entity, GRID_SQUARE);
    auto& tr2 = registry.emplace<game::Transform3DComponent>(grid.clicked_square_entity);
    tr2.scale = {0.5f, 0.5f, 0.5f};
}

void game_grid_system(game::registry& registry, double)
{
    const auto& input = *get_singleton<game::InputSingleton>(registry).input_store;
    const auto& cam = get_singleton<game::CameraSingleton>(registry);
    auto& grid = get_singleton<game::GameGridSingleton>(registry);

    auto tile_map = registry.find<game::TileMapSingleton>();
    assert(registry.valid(tile_map));

    auto& tms = registry.get<game::TileMapSingleton>(tile_map);

    // Clean out any invalid entities.
    std::erase_if(tms.tiles, [&](const auto& elem) {
        const auto& [pos, entity] = elem;
        return !registry.valid(entity);
    });

    if (input.is_mouse_clicked(spkt::Mouse::LEFT)) {
        grid.clicked_square = grid.hovered_square;
    } else if (input.is_mouse_unclicked(spkt::Mouse::RIGHT)) {
        grid.clicked_square = std::nullopt;
    }

    const auto& reg = registry;
    auto [tc, cc] = reg.get_all<game::Transform3DComponent, game::Camera3DComponent>(cam.camera_entity);

    glm::vec3 cameraPos = tc.position;
    glm::vec3 direction = spkt::Maths::GetMouseRay(
        input.mouse_position(),
        input.window_width(),
        input.window_height(),
        spkt::make_view(tc.position, tc.orientation, cc.pitch),
        spkt::make_proj(cc.fov)
    );

    float lambda = -cameraPos.y / direction.y;
    glm::vec3 mousePos = cameraPos + lambda * direction;
    grid.hovered_square = {(int)std::floor(mousePos.x), (int)std::floor(mousePos.z)};

    registry.get<game::Transform3DComponent>(grid.hovered_square_entity).position = {
        grid.hovered_square.x + 0.5f, 0.1f, grid.hovered_square.y + 0.5f
    };

    if (grid.clicked_square.has_value()) {
        auto square = grid.clicked_square.value();
        registry.get<game::Transform3DComponent>(grid.clicked_square_entity).position = {
            square.x + 0.5f, 0.1f, square.y + 0.5f
        };
    } else {
        registry.get<game::Transform3DComponent>(grid.clicked_square_entity).position = {
            0.5f, -1.0f, 0.5f
        };
    }
}