#include "sound.h"

#include <glm/glm.hpp>
#include <SFML/Audio.hpp>

namespace spkt {

struct music::impl
{
    sf::Music music;
};

struct sound::impl
{
    sf::SoundBuffer buffer;
    sf::Sound       sound;
};

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

music::music()
    : d_impl(std::make_unique<music::impl>())
{
}

music::music(const std::string& filename)
    : music()
{
    load(filename);
}

bool music::load(const std::string& filename)
{
    return d_impl->music.openFromFile(filename);
}

void music::play() const
{
    d_impl->music.play();
}

void music::pause() const
{
    d_impl->music.pause();
}

void music::stop() const
{
    d_impl->music.stop();
}

sound::sound()
    : d_impl(std::make_unique<sound::impl>())
{
}

sound::sound(const std::string& filename)
    : sound()
{
    load(filename);
}

bool sound::load(const std::string& filename)
{
    bool success = d_impl->buffer.loadFromFile(filename);
    if (success) {
        d_impl->sound.setBuffer(d_impl->buffer);
    }
    return success;
}

void sound::play_at(const glm::vec3& position)
{
    d_impl->sound.setPosition({position.x, position.y, position.z});
    d_impl->sound.play();
}

void sound::stop()
{
    d_impl->sound.stop();
}

}