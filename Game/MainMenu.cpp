#include "MainMenu.h"

#include "WorldLayer.h"
#include "EscapeMenu.h"

#include "LoadingScreen.h"

#include <future>

namespace {

void setButtonAttrs(std::shared_ptr<Sprocket::Button> button)
{
    button->base().colour = {0.15625f, 0.15625f, 0.15625f};

    button->buttonNormal().colour = {0.926f, 0.496f, 0.0f};
    button->buttonHovered().colour = {0.926f, 0.63281f, 0.3242f};
    button->buttonClicked().colour = {0.324f, 0.90625f, 0.5352f};
}

}

MainMenu::MainMenu(const Sprocket::CoreSystems& core)
    : Layer(core, Status::NORMAL, true)
    , d_container(
        core.window->width()/4.0f,
        {10.0, 10.0},
        10.0f
    )
    , d_renderer(core.window)
{
    core.window->setCursorVisibility(true);

    d_container.position({10.0f, 10.0f});
    d_container.base().colour = {0.07f, 0.07f, 0.07f};
    d_container.base().roundness = 0.081f;
    d_container.addProperty<Sprocket::HorizontalConstraint>(Sprocket::HorizontalConstraint::Type::CENTRE, 1.0f);
    d_container.addProperty<Sprocket::VerticalConstraint>(Sprocket::VerticalConstraint::Type::CENTRE, 1.0f);
}

bool MainMenu::handleEventImpl(Sprocket::Event& event)
{
    return d_container.handleEvent(d_core.window, event);
}

void MainMenu::updateImpl()
{
    d_core.window->setCursorVisibility(true);
    d_renderer.update();
    d_container.update(d_core.window);
}

void MainMenu::drawImpl()
{
    d_container.draw(&d_renderer);
}