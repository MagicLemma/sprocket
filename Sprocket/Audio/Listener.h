#pragma once
#include "ECS.h"

namespace spkt {
namespace Audio {
    
void SetListener(spkt::entity e);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}