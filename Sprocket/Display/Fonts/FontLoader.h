#pragma once
#include "Font.h"

#include <string>
#include <utility>

namespace Sprocket {

std::pair<GlyphMap, float> parseFntFile(const std::string& fntFile, const Texture& atlas);

}