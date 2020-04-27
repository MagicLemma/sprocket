#include "PlayerMovement.h"
#include "Log.h"

namespace Sprocket {

PlayerMovement::PlayerMovement(Window* window)
    : d_window(window)
    , d_deltaTime(0.0f)
{
}

void PlayerMovement::updateSystem(float dt)
{
    d_deltaTime = dt;
}

void PlayerMovement::preUpdateEntity(Entity& entity)
{

}

void PlayerMovement::postUpdateEntity(Entity& entity)
{
    if (!entity.has<PlayerComponent>()) {
        return;
    }

    auto& player = entity.get<PlayerComponent>();

    if (d_enabled) {
        player.movingForwards = d_window->isKeyDown(Keyboard::W);
        player.movingBackwards = d_window->isKeyDown(Keyboard::S);
        player.movingLeft = d_window->isKeyDown(Keyboard::A);
        player.movingRight = d_window->isKeyDown(Keyboard::D);
        player.jumping = d_window->isKeyDown(Keyboard::SPACE);
        player.yaw -= d_window->getMouseOffset().x * d_deltaTime * 10.0f;
        player.pitch -= d_window->getMouseOffset().y * d_deltaTime * 10.0f;
    }
    else {
        player.movingForwards = false;
        player.movingBackwards = false;
        player.movingLeft = false;
        player.movingRight = false;
        player.jumping = false;
    }
}

void PlayerMovement::registerEntity(const Entity& entity)
{ 
}

void PlayerMovement::deregisterEntity(const Entity& entity)
{
}

void PlayerMovement::enable(bool newEnabled)
{
    d_enabled = newEnabled;
}

}