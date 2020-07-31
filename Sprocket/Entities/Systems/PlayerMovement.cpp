#include "PlayerMovement.h"
#include "KeyboardCodes.h"
#include "EntityManager.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

PlayerMovement::PlayerMovement()
{
}

void PlayerMovement::OnUpdate(EntityManager& manager, double dt)
{
    d_mouse.OnUpdate();

    manager.Each<PlayerComponent, PhysicsComponent>([&](Entity& entity) {
        auto& player = entity.Get<PlayerComponent>();
        auto& physics = entity.Get<PhysicsComponent>();

        float sensitivity = 0.15f;

        if (d_enabled) {
            player.movingForwards = d_keyboard.IsKeyDown(Keyboard::W);
            player.movingBackwards = d_keyboard.IsKeyDown(Keyboard::S);
            player.movingLeft = d_keyboard.IsKeyDown(Keyboard::A);
            player.movingRight = d_keyboard.IsKeyDown(Keyboard::D);
            player.jumping = d_keyboard.IsKeyDown(Keyboard::SPACE);
            
            player.yaw -= d_mouse.GetMouseOffset().x * sensitivity;
            player.pitch -= d_mouse.GetMouseOffset().y * sensitivity;

            Maths::Clamp(player.pitch, -89.0f, 89.0f);

            if (entity.Has<CameraComponent>()) {
                entity.Get<CameraComponent>().pitch -=d_mouse.GetMouseOffset().y * sensitivity;
                Maths::Clamp(entity.Get<CameraComponent>().pitch, -89.0f, 89.0f);
            }
        }
        else {
            player.movingForwards = false;
            player.movingBackwards = false;
            player.movingLeft = false;
            player.movingRight = false;
            player.jumping = false;
        }

        // Update the direction
        float cosYaw = Maths::Cosd(player.yaw);
        float sinYaw = Maths::Sind(player.yaw);

        Maths::vec3 forwards(-sinYaw, 0, -cosYaw);
        Maths::vec3 right(cosYaw, 0, -sinYaw);
        Maths::vec3 direction(0.0f, 0.0f, 0.0f);

        if (player.movingForwards) { direction += forwards; }
        if (player.movingBackwards) { direction -= forwards; }
        if (player.movingRight) { direction += right; }
        if (player.movingLeft) { direction -= right; }
        
        Maths::Normalise(direction);
        player.direction = direction;
    });
}

void PlayerMovement::OnEvent(Event& event)
{
    d_keyboard.OnEvent(event);
    d_mouse.OnEvent(event);
}

void PlayerMovement::Enable(bool newEnabled)
{
    d_enabled = newEnabled;
    d_keyboard.ConsumeAll(d_enabled);
}

}