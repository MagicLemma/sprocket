#include "PlayerMovement.h"
#include "KeyboardCodes.h"
#include "Scene.h"
#include "Components.h"
#include "Log.h"

#include <cmath>

namespace Sprocket {

PlayerMovement::PlayerMovement(PhysicsEngine* physicsEngine)
    : d_physicsEngine(physicsEngine)
{
}

void PlayerMovement::OnUpdate(Scene& scene, double dt)
{
    d_mouse.OnUpdate();

    scene.Each<PlayerComponent, PhysicsComponent>([&](Entity& entity) {
        auto& player = entity.Get<PlayerComponent>();
        auto& physics = entity.Get<PhysicsComponent>();

        if (!d_enabled) { return; }

        player.yaw -= d_mouse.GetMouseOffset().x * 0.15f;

        if (entity.Has<CameraComponent>()) {
            entity.Get<CameraComponent>().pitch -= d_mouse.GetMouseOffset().y * 0.15f;
            Maths::Clamp(entity.Get<CameraComponent>().pitch, -89.0f, 89.0f);
        }

        d_physicsEngine->MakeUpright(entity, player.yaw);
        
        // Update the direction
        float cosYaw = Maths::Cosd(player.yaw);
        float sinYaw = Maths::Sind(player.yaw);

        Maths::vec3 forwards(-sinYaw, 0, -cosYaw);
        Maths::vec3 right(cosYaw, 0, -sinYaw);

        Maths::vec3 direction(0.0f, 0.0f, 0.0f);
        if (d_keyboard.IsKeyDown(Keyboard::W)) { direction += forwards; }
        if (d_keyboard.IsKeyDown(Keyboard::S)) { direction -= forwards; }
        if (d_keyboard.IsKeyDown(Keyboard::D)) { direction += right; }
        if (d_keyboard.IsKeyDown(Keyboard::A)) { direction -= right; }
        Maths::Normalise(direction);

        float pdt = d_physicsEngine->LastFrameLength(); // Physics dt
        if (pdt == 0) {
            return; // Physics engine not advanced this frame.
        }

        bool onFloor = d_physicsEngine->IsOnFloor(entity);
        if (direction.length() != 0.0f || onFloor) {
            float speed = 3.0f;
            Maths::vec3 dv = (speed * direction) - physics.velocity;
            dv.y = 0.0f;  // Only consider horizontal movement.
            Maths::vec3 acceleration = dv / pdt;
            d_physicsEngine->ApplyForce(entity, physics.mass * acceleration);
        }

        // Jumping
        if (onFloor && d_keyboard.IsKeyDown(Keyboard::SPACE)) {
            float speed = 6.0f;
            Maths::vec3 dv = (speed - physics.velocity.y) * Maths::vec3(0, 1, 0);
            Maths::vec3 acceleration = dv / pdt;
            d_physicsEngine->ApplyForce(entity, physics.mass * acceleration);
        }
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