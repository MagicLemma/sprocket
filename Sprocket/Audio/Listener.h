#pragma once
#include "Entity.h"
#include "Event.h"

namespace Sprocket {
namespace Audio {
    
void SetListener(const Entity& entity);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}