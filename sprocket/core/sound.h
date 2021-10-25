#pragma once
#include <glm/glm.hpp>

#include <string>
#include <memory>

namespace spkt {

void set_listener(const glm::vec3& postiion, const glm::vec3& direction);

float get_master_volume();
void set_master_volume(float volume);

class music
{
    struct impl;
    std::unique_ptr<impl> d_impl;

public:
    music();
    music(const std::string& filename);
    ~music() = default;

    bool load(const std::string& filename);
    void play() const;
    void pause() const;
    void stop() const;
};

// Make it possible to run in multiple places, may need to split back out into "source"
// and "sound", but lets keep it super simple for now.
class sound
{
    struct impl;
    std::unique_ptr<impl> d_impl;

public:
    sound();
    sound(const std::string& filename);
    ~sound() = default;

    bool load(const std::string& filename);
    void play_at(const glm::vec3& position);
    void stop();
};

}