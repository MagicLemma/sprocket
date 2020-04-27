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
    return Maths::inverse(Maths::transform(t.position, t.orientation));
}

void PlayerBasedCamera::update(Window* window, float timeDelta)
{
}

}