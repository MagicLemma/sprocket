#include "Listener.h"
#include "Components.h"

#include <SFML/Audio.hpp>

namespace Sprocket {
namespace Audio {

void SetListener(const ecs::Entity& entity)
{
    if (!entity.Has<Transform3DComponent>()) { return; }
    auto tr = entity.Get<Transform3DComponent>();
    auto pos = tr.position;
    sf::Listener::setPosition(pos.x, pos.y, pos.z);

    auto dir = Maths::Forwards(tr.orientation);
    sf::Listener::setDirection(dir.x, dir.y, dir.z);
}

float GetMasterVolume()
{
    return sf::Listener::getGlobalVolume();
}

void SetMasterVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
}

}
}