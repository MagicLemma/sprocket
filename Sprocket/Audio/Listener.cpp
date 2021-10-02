#include "Listener.h"

#include <SFML/Audio.hpp>

namespace spkt {

void set_listener(const glm::vec3& postiion, const glm::vec3& direction)
{
    sf::Listener::setPosition(postiion.x, postiion.y, postiion.z);
    sf::Listener::setDirection(direction.x, direction.y, direction.z);
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