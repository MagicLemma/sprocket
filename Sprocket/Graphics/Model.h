#pragma once
#include <memory>

namespace Sprocket {

enum class ModelType
{
    ENTITY,
        // Declares that the model is a 3D entity with three
        // attributes; position, texture and normal.
    FLAT
        // Delares that the model is a 2D flat model with one
        // attribute; position. This is used for GUI elements.
};

class Model
{
    unsigned int d_vaoId;
    int          d_vertexCount;
    ModelType    d_type;

public:
    Model(unsigned int vaoId, int vertexCount, ModelType type);

    unsigned int vaoId() const { return d_vaoId; }
    int vertexCount() const { return d_vertexCount; }

    void bind() const;
    void unbind() const;
};

using ModelPtr = std::shared_ptr<Model>;

}