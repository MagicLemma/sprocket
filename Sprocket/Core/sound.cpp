#include "sound.h"

#include <glm/glm.hpp>
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

struct MusicImpl
{
    sf::Music music;
};

music::music()
    : d_impl(std::make_unique<MusicImpl>())
{
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

struct SoundImpl
{
    sf::SoundBuffer buffer;
};

Sound::Sound()
    : d_impl(std::make_unique<SoundImpl>())
{
}

bool Sound::Load(const std::string& filename)
{
    return d_impl->buffer.loadFromFile(filename);
}

struct SourceImpl
{
    sf::Sound sound;
}; 

Source::Source()
    : d_impl(std::make_unique<SourceImpl>())
{
}


void Source::SetSound(const Sound& sound)
{
    d_impl->sound.setBuffer(sound.d_impl->buffer);
}

void Source::SetPosition(float x, float y, float z)
{
    d_impl->sound.setPosition({x, y, z});
}

void Source::SetPosition(const glm::vec3& position)
{
    SetPosition(position.x, position.y, position.z);
}

void Source::Play() const
{
    d_impl->sound.play();
}

void Source::Pause() const
{
    d_impl->sound.pause();
}

void Source::Stop() const
{
    d_impl->sound.stop();
}

}