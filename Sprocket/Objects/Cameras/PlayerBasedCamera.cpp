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
    return Maths::inverse(d_player->getComponent<TransformComponent>().transform);
}

void PlayerBasedCamera::update(Window* window, float timeDelta)
{
}

}