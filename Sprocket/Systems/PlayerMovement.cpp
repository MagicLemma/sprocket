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
    d_mouse.update();
}

void PlayerMovement::updateEntity(float dt, Entity& entity)
{
    if (!entity.has<PlayerComponent>()) {
        return;
    }

    float sensitivity = 0.15f;

    auto& player = entity.get<PlayerComponent>();
    const auto& physics = entity.get<PhysicsComponent>();

    if (d_enabled) {
        player.movingForwards = d_keyboard.isKeyDown(Keyboard::W);
        player.movingBackwards = d_keyboard.isKeyDown(Keyboard::S);
        player.movingLeft = d_keyboard.isKeyDown(Keyboard::A);
        player.movingRight = d_keyboard.isKeyDown(Keyboard::D);
        player.jumping = d_keyboard.isKeyDown(Keyboard::SPACE);
        
        player.yaw -= d_mouse.getMouseOffset().x * sensitivity;
        player.pitch -= d_mouse.getMouseOffset().y * sensitivity;
        Maths::clamp(player.pitch, -89.0f, 89.0f);
    }
    else {
        player.movingForwards = false;
        player.movingBackwards = false;
        player.movingLeft = false;
        player.movingRight = false;
        player.jumping = false;
    }

    // Update the direction
    float cosYaw = Maths::cosd(player.yaw);
    float sinYaw = Maths::sind(player.yaw);

    Maths::vec3 forwards(-sinYaw, 0, -cosYaw);
    Maths::vec3 right(cosYaw, 0, -sinYaw);
    Maths::vec3 direction(0.0f, 0.0f, 0.0f);

    if (player.movingForwards) { direction += forwards; }
    if (player.movingBackwards) { direction -= forwards; }
    if (player.movingRight) { direction += right; }
    if (player.movingLeft) { direction -= right; }
    
    Maths::normalise(direction);
    player.direction = direction;
}

void PlayerMovement::handleEvent(Event& event)
{
    d_keyboard.handleEvent(event);
    d_mouse.handleEvent(event);
}

void PlayerMovement::enable(bool newEnabled)
{
    d_enabled = newEnabled;
}

}