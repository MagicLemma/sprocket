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

    Maths::quat orientation = d_player->Orientation() *
                              Maths::Rotate({1, 0, 0}, p.pitch);

    return Maths::Inverse(Maths::Transform(position, orientation));
}

}