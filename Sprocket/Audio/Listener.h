#pragma once
#include "ECS.h"

namespace Sprocket {
namespace Audio {
    
void SetListener(spkt::entity e);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}