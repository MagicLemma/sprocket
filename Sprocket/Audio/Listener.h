#pragma once
#include "ECS.h"

namespace spkt {
namespace Audio {
    
void SetListener(apx::handle e);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}