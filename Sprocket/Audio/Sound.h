#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace spkt {
namespace Audio {

struct MusicImpl;
struct SoundImpl;
struct SourceImpl;

class Music
{
    std::unique_ptr<MusicImpl> d_impl;

public:
    Music();
    bool Load(const std::string& filename);
    
    void Play() const;
    void Pause() const;
    void Stop() const;
};

class Sound
{
    std::unique_ptr<SoundImpl> d_impl;

public:
    Sound();
    bool Load(const std::string& filename);
    friend class Source;
};


class Source
{
    std::unique_ptr<SourceImpl> d_impl;

public:
    Source();

    Source(Source&&) = delete;
    Source& operator=(const Source&) = delete;
        // Non-copyable

    void SetSound(const Sound& sound);

    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& position);

    void Play() const;
    void Pause() const;
    void Stop() const;
};

}
}