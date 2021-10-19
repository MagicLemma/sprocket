#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace spkt {

struct MusicImpl;
struct SoundImpl;
struct SourceImpl;

void set_listener(const glm::vec3& postiion, const glm::vec3& direction);

float get_master_volume();
void set_master_volume(float volume);

class music
{
    std::unique_ptr<MusicImpl> d_impl;

public:
    music();

    bool load(const std::string& filename);
    void play() const;
    void pause() const;
    void stop() const;
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

    void SetSound(const Sound& sound);

    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& position);

    void Play() const;
    void Pause() const;
    void Stop() const;
};

}