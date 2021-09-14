#include "game_grid.h"

#include <Sprocket/Scene/Camera.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Utility/Maths.h>
#include <Sprocket/Core/input_codes.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cassert>

namespace spkt {
namespace {

constexpr const char* GRID_SQUARE = "Resources/Models/Square.obj";

}

void game_grid_system_init(spkt::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    registry.emplace<CameraSingleton>(singleton);
    auto& grid = registry.emplace<GameGridSingleton>(singleton);

    grid.hovered_square_entity = registry.create();
    registry.emplace<Runtime>(grid.hovered_square_entity);
    registry.emplace<NameComponent>(grid.hovered_square_entity, "Hovered Grid Highlighter");
    registry.emplace<StaticModelComponent>(grid.hovered_square_entity, GRID_SQUARE);
    auto& tr1 = registry.emplace<Transform3DComponent>(grid.hovered_square_entity);
    tr1.scale = {0.3f, 0.3f, 0.3f};

    grid.clicked_square_entity = registry.create();
    registry.emplace<Runtime>(grid.clicked_square_entity);
    registry.emplace<NameComponent>(grid.clicked_square_entity, "Clicked Grid Highlighter");
    registry.emplace<StaticModelComponent>(grid.clicked_square_entity, GRID_SQUARE);
    auto& tr2 = registry.emplace<Transform3DComponent>(grid.clicked_square_entity);
    tr2.scale = {0.5f, 0.5f, 0.5f};
}

void game_grid_system(spkt::registry& registry, double)
{
    const auto& input = get_singleton<InputSingleton>(registry);
    const auto& cam = get_singleton<CameraSingleton>(registry);
    auto& grid = get_singleton<GameGridSingleton>(registry);

    auto tile_map = registry.find<TileMapSingleton>();
    assert(registry.valid(tile_map));

    auto& tms = registry.get<TileMapSingleton>(tile_map);

    // Clean out any invalid entities.
    std::erase_if(tms.tiles, [&](const auto& elem) {
        const auto& [pos, entity] = elem;
        return !registry.valid(entity);
    });

    if (input.mouse_click[Mouse::LEFT]) {
        grid.clicked_square = grid.hovered_square;
    } else if (input.mouse_click[Mouse::RIGHT]) {
        grid.clicked_square = std::nullopt;
    }

    auto& camTr = registry.get<Transform3DComponent>(cam.camera_entity);

    glm::vec3 cameraPos = camTr.position;
    glm::vec3 direction = Maths::GetMouseRay(
        input.mouse_pos,
        input.window_width,
        input.window_height,
        spkt::make_view(registry, cam.camera_entity),
        spkt::make_proj(registry, cam.camera_entity)
    );

    float lambda = -cameraPos.y / direction.y;
    glm::vec3 mousePos = cameraPos + lambda * direction;
    grid.hovered_square = {(int)std::floor(mousePos.x), (int)std::floor(mousePos.z)};

    registry.get<Transform3DComponent>(grid.hovered_square_entity).position = {
        grid.hovered_square.x + 0.5f, 0.1f, grid.hovered_square.y + 0.5f
    };

    if (grid.clicked_square.has_value()) {
        auto square = grid.clicked_square.value();
        registry.get<Transform3DComponent>(grid.clicked_square_entity).position = {
            square.x + 0.5f, 0.1f, square.y + 0.5f
        };
    } else {
        registry.get<Transform3DComponent>(grid.clicked_square_entity).position = {
            0.5f, -1.0f, 0.5f
        };
    }
}

}