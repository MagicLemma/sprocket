#pragma once
#include "ECS.h"
#include "Event.h"

namespace Sprocket {
namespace Audio {
    
void SetListener(const ECS::Entity& entity);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}