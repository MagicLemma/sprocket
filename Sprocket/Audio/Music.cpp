#include "Music.h"

#include <SFML/Audio.hpp>

namespace Sprocket {
namespace Audio {

struct MusicImpl
{
    sf::Music music;
};

Music::Music()
    : d_impl(std::make_shared<MusicImpl>())
{
}

bool Music::Load(const std::string& filename)
{
    return d_impl->music.openFromFile(filename);
}

void Music::Play() const
{
    return;
    d_impl->music.play();
}

void Music::Pause() const
{
    return;
    d_impl->music.pause();
}

void Music::Stop() const
{
    return;
    d_impl->music.stop();
}

}
}