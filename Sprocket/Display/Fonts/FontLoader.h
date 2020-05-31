#pragma once
#include "Font.h"

#include <string>
#include <utility>

namespace Sprocket {

std::pair<GlyphMap, float> ParseFntFile(
    const std::string& fntFile,
    const Texture& atlas);

}