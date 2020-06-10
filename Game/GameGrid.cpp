#include "GameGrid.h"

using namespace Sprocket;

namespace {

Model3D GetHoveredSquare()
{
    Vertex3DBuffer vb = {
        {{-1, 0, 1},  {0, 1, 0}, {0, 0}}, // Top left
        {{1, 0, 1},   {0, 1, 0}, {1, 0}}, // Top right
        {{-1, 0, -1}, {0, 1, 0}, {0, 1}}, // Bottom left
        {{1, 0, -1},  {0, 1, 0}, {1, 1}}  // Bottom right
    };
    IndexBuffer ib = { 0, 1, 2, 3, 2, 1 };
    return Model3D(vb, ib);
}

}

GameGrid::GameGrid()
    : d_highlightSquare(std::make_shared<Entity>())
{
    auto model = d_highlightSquare->Add<ModelComponent>();
    model->model = GetHoveredSquare();
    model->material.texture = Texture::White();
    model->material.reflectivity = 0.0f;
    model->scale = 0.5f;
}

void GameGrid::OnUpdate(Window* window, Camera* camera, Lens* lens)
{
    d_mouse.OnUpdate();
    Maths::vec3 cameraPos = camera->Position();
    Maths::vec3 direction = Maths::GetMouseRay(
        d_mouse.GetMousePos(),
        window->Width(),
        window->Height(),
        camera->View(),
        lens->Projection()
    );

    float lambda = -cameraPos.y / direction.y;
    Maths::vec3 mousePos = cameraPos + lambda * direction;
    d_x = (int)std::floor(mousePos.x);
    d_z = (int)std::floor(mousePos.z);

    d_highlightSquare->Position() = { d_x + 0.5f, 0.05f, d_z + 0.5f };
}

void GameGrid::OnEvent(Event& event)
{
    d_mouse.OnEvent(event);
    d_keyboard.OnEvent(event);
}

std::shared_ptr<Entity> GameGrid::GetHighlightSquare() const
{
    return d_highlightSquare;
}