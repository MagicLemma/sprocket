#include "GameGrid.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "MouseCodes.h"
#include "Log.h"

#include <random>
#include <cassert>

namespace Sprocket {

std::string Name(const spkt::entity& e) {
    if (e.has<NameComponent>()) {
        return e.get<NameComponent>().name;
    }
    return "Entity";
}

GameGrid::GameGrid()
    : d_hovered({0.0, 0.0})
    , d_selected({})
{
}

void GameGrid::on_startup(spkt::registry& registry)
{
    auto singleton = registry.find<Singleton>();
    auto& grid = registry.emplace<GameGridSingleton>(singleton);
    registry.emplace<CameraSingleton>(singleton);

    std::string gridSquare = "Resources/Models/Square.obj";

    grid.hovered_square_entity = registry.create();
    registry.emplace<NameComponent>(grid.hovered_square_entity, "Hovered Grid Highlighter");
    registry.emplace<TemporaryComponent>(grid.hovered_square_entity);
    auto& tr1 = registry.emplace<Transform3DComponent>(grid.hovered_square_entity);
    tr1.scale = {0.3f, 0.3f, 0.3f};
    auto& model1 = registry.emplace<ModelComponent>(grid.hovered_square_entity);
    model1.mesh = gridSquare;

    grid.clicked_square_entity = registry.create();
    registry.emplace<NameComponent>(grid.clicked_square_entity, "Clicked Grid Highlighter");
    registry.emplace<TemporaryComponent>(grid.clicked_square_entity);
    auto& tr2 = registry.emplace<Transform3DComponent>(grid.clicked_square_entity);
    tr2.scale = {0.5f, 0.5f, 0.5f};
    auto& model2 = registry.emplace<ModelComponent>(grid.clicked_square_entity);
    model2.mesh = gridSquare;
}

void GameGrid::on_event(spkt::registry& registry, ev::Event& event)
{
    auto singleton = registry.find<Singleton>();
    if (!registry.valid(singleton)) {
        return;
    }

    auto& grid = registry.get<GameGridSingleton>(singleton);

    if (auto e = event.get_if<spkt::added<GridComponent>>()) {
        spkt::entity entity = e->entity;
        auto& transform = entity.get<Transform3DComponent>();
        const auto& gc = entity.get<GridComponent>();

        assert(!grid.game_grid.contains({gc.x, gc.z}));
    
        transform.position.x = gc.x + 0.5f;
        transform.position.z = gc.z + 0.5f;
        grid.game_grid[{gc.x, gc.z}] = entity.entity();
    }
    else if (auto e = event.get_if<spkt::removed<GridComponent>>()) {
        auto& gc = e->entity.get<GridComponent>();
        auto it = grid.game_grid.find({gc.x, gc.z});
        if (it == grid.game_grid.end()) {
            log::warn("No entity exists at this coord!");
        }
        else {
            grid.game_grid.erase(it);
        }
    }
}

void GameGrid::on_update(spkt::registry& registry, double dt)
{
    const auto& input = get_singleton<InputSingleton>(registry);
    const auto& cam = get_singleton<CameraSingleton>(registry);
    auto& grid = get_singleton<GameGridSingleton>(registry);

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
        MakeView({registry, cam.camera_entity}),
        MakeProj({registry, cam.camera_entity})
    );

    float lambda = -cameraPos.y / direction.y;
    glm::vec3 mousePos = cameraPos + lambda * direction;
    grid.hovered_square = {(int)std::floor(mousePos.x), (int)std::floor(mousePos.z)};

    registry.get<Transform3DComponent>(grid.hovered_square_entity).position = {
        grid.hovered_square.x + 0.5f, 0.05f, grid.hovered_square.y + 0.5f
    };

    if (grid.clicked_square.has_value()) {
        auto square = grid.clicked_square.value();
        registry.get<Transform3DComponent>(grid.clicked_square_entity).position = {
            square.x + 0.5f, 0.05f, square.y + 0.5f
        };
    } else {
        registry.get<Transform3DComponent>(grid.clicked_square_entity).position = {
            0.5f, -1.0f, 0.5f
        };
    }
}

spkt::entity GameGrid::At(spkt::registry& registry, const glm::ivec2& pos) const
{
    const auto& grid = get_singleton<GameGridSingleton>(registry);

    auto it = grid.game_grid.find(pos);
    if (it != grid.game_grid.end()) {
        return {registry, it->second};
    }
    return spkt::null;
}

spkt::entity GameGrid::Hovered(spkt::registry& registry) const
{
    const auto& grid = get_singleton<GameGridSingleton>(registry);
    return At(registry, grid.hovered_square);
}

spkt::entity GameGrid::Selected(spkt::registry& registry) const
{
    const auto& grid = get_singleton<GameGridSingleton>(registry);

    if (grid.clicked_square.has_value()) {
        return At(registry, grid.clicked_square.value());
    }
    return spkt::null;
}

}