#include "UILayer.h"

UILayer::UILayer(std::shared_ptr<BasicSceneInfo> info) 
    : Layer(Status::INACTIVE, true)
    , d_info(info)
    , d_displayRenderer(info->window)
{
    Sprocket::Vertex2DBuffer v = {
        {Sprocket::Maths::vec2{0.5f, 0.5f}, Sprocket::Maths::vec2{1.0f, 1.0f}},
        {Sprocket::Maths::vec2{-0.5f, -0.5f}, Sprocket::Maths::vec2{0.0f, 0.0f}},
        {Sprocket::Maths::vec2{-0.5f, 0.5f}, Sprocket::Maths::vec2{0.0f, 1.0f}},
        {Sprocket::Maths::vec2{0.5f, 0.5f}, Sprocket::Maths::vec2{1.0f, 1.0f}},
        {Sprocket::Maths::vec2{0.5f, -0.5f}, Sprocket::Maths::vec2{1.0f, 0.0f}},
        {Sprocket::Maths::vec2{-0.5f, -0.5f}, Sprocket::Maths::vec2{0.0f, 0.0f}}
    };
    auto tri = Sprocket::Loader::loadModel2D(v);
    d_info->models.push_back(tri);
}

bool UILayer::handleEventImpl(Sprocket::Window* window, const Sprocket::Event& event)
{
    if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
        if (e->key() == Sprocket::Keyboard::ESC) {
            d_info->paused = !d_info->paused;
            window->setCursorVisibility(d_info->paused);
            return true;
        }
    }
    else if (auto e = event.as<Sprocket::MouseButtonPressedEvent>()) {
        auto pos = window->getMousePos();
        if (pos.x < 50) {
            d_info->paused = false;
            window->setCursorVisibility(false);
        }
    }
    return false;
}

void UILayer::updateImpl(Sprocket::Window* window)
{
    d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;
}

void UILayer::drawImpl(Sprocket::Window* window)
{
    Sprocket::RenderOptions options;
    options.faceCulling = false;
    options.depthTest = false;
    for (const auto& model: d_info->models) {
        d_displayRenderer.draw(model, options);
    }
}