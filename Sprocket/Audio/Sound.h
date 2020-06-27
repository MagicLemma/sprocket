#pragma once
#include <string>
#include <memory>

namespace Sprocket {
namespace Audio {

struct SoundImpl;
struct SourceImpl;

class Sound
{
    std::shared_ptr<SoundImpl> d_impl;

public:
    Sound();
    bool Load(const std::string& filename);
    friend class Source;
};


class Source
{
    std::shared_ptr<SourceImpl> d_impl;

public:
    Source();

    Source(Source&&) = delete;
    Source& operator=(const Source&) = delete;
        // Non-copyable

    void SetSound(const Sound& sound);

    void Play() const;
    void Pause() const;
    void Stop() const;
};

}
}