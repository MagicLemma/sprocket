#include "Listener.h"

#include <SFML/Audio.hpp>

namespace Sprocket {
namespace Audio {

void SetListener(const Entity& entity)
{
    auto pos = entity.Position();
    sf::Listener::setPosition(pos.x, pos.y, pos.z);

    auto dir = Maths::Forwards(entity.Orientation());
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