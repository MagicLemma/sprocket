#pragma once
#include <Sprocket.h>

#include <memory>

class GameGrid
{
    Sprocket::KeyboardProxy d_keyboard;
    Sprocket::MouseProxy    d_mouse;

    std::shared_ptr<Sprocket::Entity> d_highlightSquare;
    int d_x;
    int d_z;

public:
    GameGrid();

    void OnUpdate(Sprocket::Window* window,
                  Sprocket::Camera* camera,
                  Sprocket::Lens* lens);

    void OnEvent(Sprocket::Event& event);

    std::shared_ptr<Sprocket::Entity> GetHighlightSquare() const;
};