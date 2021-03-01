#pragma once
#include <string>
#include <string_view>
#include <memory>

namespace Sprocket {
namespace Audio {

struct MusicImpl;

class Music
{
    std::unique_ptr<MusicImpl> d_impl;

public:
    Music();
    bool Load(std::string_view filename);
    
    void Play() const;
    void Pause() const;
    void Stop() const;
};

}
}