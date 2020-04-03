#include "Font.h"
#include "FontLoader.h"

namespace Sprocket {

std::string getFontName(Font font)
{
    switch (font) {
        case Font::ARIAL: return "Arial";
        case Font::CALIBRI: return "Calibri";
        case Font::GEORGIA: return "Georgia";
    }    
    return "";
}

FontPackage::FontPackage(ResourceManager* resourceManager,
                         const std::string& name,
                         const std::string& fntFile, const std::string& texFile)
    : d_name(name)
    , d_atlas(resourceManager->loadTexture(texFile))
    , d_glyphs()
    , d_size(0.0f)
{
    auto [glyphs, size] = parseFntFile(resourceManager, fntFile, d_atlas);
    d_glyphs = glyphs;
    d_size = size;
}

Character FontPackage::get(int id) const
{
    auto it = d_glyphs.find(id);
    if (it == d_glyphs.end()) {
        return d_glyphs.find(0)->second;
    }
    return it->second;
}
    
}