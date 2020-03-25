#pragma once
#include "Font.h"

#include <string>

namespace Sprocket {

CharacterMap parseFntFile(const std::string& fntFile, const Texture& atlas);

Font loadFont(const std::string& fntFile,
              const std::string& texFile);

}