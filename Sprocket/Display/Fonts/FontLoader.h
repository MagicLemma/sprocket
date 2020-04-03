#pragma once
#include "Font.h"
#include "ResourceManager.h"

#include <string>
#include <utility>

namespace Sprocket {

std::pair<GlyphMap, float> parseFntFile(
    ResourceManager* resourceManager,
    const std::string& fntFile,
    const Texture& atlas);

}