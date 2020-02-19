#include "TexturedModel.h"

namespace Sprocket {

TexturedModel::TexturedModel(const RawModel& rawModel, const Texture& texture)
    : d_rawModel(rawModel)
    , d_texture(texture)
    {}

}