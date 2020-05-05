#include "LoadingScreen.h"

#include "WorldLayer.h"
#include "EscapeMenu.h"

LoadingScreen::LoadingScreen(Sprocket::Accessor& accessor)
    : Layer(accessor, Status::NORMAL, true)
    , d_container(
        accessor.window()->width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_renderer(accessor.window())
{
    accessor.window()->setCursorVisibility(true);

    d_container.position({10.0f, 10.0f});
    d_container.base().colour = {0.07f, 0.07f, 0.07f};
    d_container.base().roundness = 0.081f;
    d_container.addProperty<Sprocket::HorizontalConstraint>(Sprocket::HorizontalConstraint::Type::CENTRE, 1.0f);
    d_container.addProperty<Sprocket::VerticalConstraint>(Sprocket::VerticalConstraint::Type::CENTRE, 1.0f);

    auto text = d_container.add<Sprocket::TextBox>(300.0f, 300.0f, "Loading...");
    text->base().colour = {0.15625f, 0.15625f, 0.15625f};
}

bool LoadingScreen::handleEventImpl(const Sprocket::Event& event)
{
    return d_container.handleEvent(d_accessor.window(), event);
}

void LoadingScreen::updateImpl()
{
    d_accessor.window()->setCursorVisibility(true);
    d_renderer.update();
    d_container.update(d_accessor.window());
}

void LoadingScreen::drawImpl()
{
    d_container.draw(&d_renderer);
}