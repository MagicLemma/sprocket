#include "Listener.h"

#include <SFML/Audio.hpp>

namespace Sprocket {

Listener::Listener(Entity* camera)
    : d_camera(camera)
{   
}

void Listener::OnUpdate()
{
    if (d_camera == nullptr) {
        return;
    }

    auto pos = d_camera->Position();
    sf::Listener::setPosition(pos.x, pos.y, pos.z);

    auto dir = Maths::Forwards(d_camera->Orientation());
    sf::Listener::setDirection(dir.x, dir.y, dir.z);
}

}