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
    Maths::mat4 t = Maths::transform(
        d_player->get<TransformComponent>().position,
        d_player->get<TransformComponent>().orientation
    );
    return Maths::inverse(t);
}

void PlayerBasedCamera::update(Window* window, float timeDelta)
{
}

}