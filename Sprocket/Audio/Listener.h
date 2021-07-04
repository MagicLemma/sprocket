#pragma once
#include "apecs.hpp"

namespace spkt {
namespace Audio {
    
void SetListener(apx::handle e);

float GetMasterVolume();
void SetMasterVolume(float volume);

}
}