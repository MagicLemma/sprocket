#include "PlayerBasedCamera.h"
#include "WindowEvent.h"

#include <cmath>
#include <algorithm>

namespace Sprocket {

PlayerBasedCamera::PlayerBasedCamera(Entity* player)
    : d_player(player)
{
}

Maths::mat4 PlayerBasedCamera::View() const
{
    auto p = d_player->Get<PlayerComponent>();

    Maths::vec3 position = d_player->Position();
    Maths::quat orientation = Maths::identity;
    orientation = Maths::Rotate(orientation, {1, 0, 0}, p.pitch);
    orientation = Maths::Rotate(orientation, {0, 1, 0}, p.yaw);
    
    return Maths::Inverse(Maths::Transform(position, orientation));
}

}