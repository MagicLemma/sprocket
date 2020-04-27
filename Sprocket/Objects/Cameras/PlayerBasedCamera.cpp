#include "PlayerBasedCamera.h"
#include "WindowEvent.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

PlayerBasedCamera::PlayerBasedCamera(Entity* player)
    : d_player(player)
{
}

Maths::mat4 PlayerBasedCamera::view() const
{
    auto t = d_player->get<TransformComponent>();
    auto p = d_player->get<PlayerComponent>();

    Maths::vec3 position = t.position;
    Maths::mat3 orientation(1.0f);
    orientation = Maths::rotate(orientation, {0, 1, 0}, p.yaw);
    orientation = Maths::rotate(orientation, {1, 0, 0}, p.pitch);
    
    return Maths::inverse(Maths::transform(position, orientation));
}

void PlayerBasedCamera::update(Window* window, float timeDelta)
{
}

}