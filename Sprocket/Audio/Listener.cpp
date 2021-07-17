#include "Listener.h"

#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Utility/Maths.h>

#include <SFML/Audio.hpp>

namespace spkt {

void set_listener(const spkt::registry& registry, spkt::entity entity)
{
    if (!registry.has<Transform3DComponent>(entity)) { return; }
    auto tr = registry.get<Transform3DComponent>(entity);
    auto pos = tr.position;
    sf::Listener::setPosition(pos.x, pos.y, pos.z);

    auto dir = Maths::Forwards(tr.orientation);
    sf::Listener::setDirection(dir.x, dir.y, dir.z);
}

float get_master_volume()
{
    return sf::Listener::getGlobalVolume();
}

void set_master_volume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
}

}