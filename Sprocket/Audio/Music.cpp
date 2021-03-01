#include "Music.h"

#include <SFML/Audio.hpp>

namespace Sprocket {
namespace Audio {

struct MusicImpl
{
    sf::Music music;
};

Music::Music()
    : d_impl(std::make_unique<MusicImpl>())
{
}

bool Music::Load(std::string_view filename)
{
    return d_impl->music.openFromFile(std::string(filename));
}

void Music::Play() const
{
    d_impl->music.play();
}

void Music::Pause() const
{
    d_impl->music.pause();
}

void Music::Stop() const
{
    d_impl->music.stop();
}

}
}