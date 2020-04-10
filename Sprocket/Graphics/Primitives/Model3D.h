#pragma once
#include "Maths.h"
#include "Resources.h"

#include <vector>
#include <string>

namespace Sprocket {

struct Vertex3D
{
    Maths::vec3 position;
    Maths::vec3 normal;
    Maths::vec2 textureCoords;
    float       textureIndex;
};

using Vertex3DBuffer = std::vector<Vertex3D>;
using IndexBuffer = std::vector<unsigned int>;

class Model3D
{
    // GPU STORAGE
    std::shared_ptr<VAO> d_vao;
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    // CPU STORAGE
    std::shared_ptr<Vertex3DBuffer> d_vertexData;
    std::shared_ptr<IndexBuffer>    d_indexData;

    std::shared_ptr<VBO> loadVertexBuffer(const Vertex3DBuffer& vertices);
    std::shared_ptr<VBO> loadIndexBuffer(const IndexBuffer& indices);

public:
    // CONSTRUCTORS
    Model3D(const std::string& objFile);

    Model3D(const Vertex3DBuffer& vertices,
            const IndexBuffer& indices);

        // If storeGPU is true, a VAO and two VBOs are created, and the data
        // is loaded in and the layouts set. These may be used by binding the
        // model.

        // If store CPU is true, the vertex buffer and index buffer vectors
        // are copied into this object and stored. These may be retreived by
        // the getter functions below. This is useful for batch rendering.

    Model3D(); // Empty model

    int vertexCount() const { return d_indexData->size(); }

    // GPU STORAGE ACCESS
    void bind() const;
    void unbind() const;

    // CPU STORAGE ACCESS
    const Vertex3DBuffer& vertexBuffer() const { return *d_vertexData.get(); }
    const IndexBuffer& indexBuffer() const { return *d_indexData.get(); }

    bool operator==(const Model3D& other) const;
};

}