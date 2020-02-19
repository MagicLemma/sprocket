#pragma once
#include "RawModel.h"
#include "../Texture.h"

namespace Sprocket {

class TexturedModel
{
    RawModel d_rawModel;
    Texture  d_texture;

public:
    TexturedModel(const RawModel& rawModel, const Texture& texture);
};

}