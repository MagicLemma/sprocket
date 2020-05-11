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
    Model3D(const Vertex3DBuffer& vertices,
            const IndexBuffer& indices);

    Model3D(); // Empty model

    std::size_t vertexCount() const { return d_indexData->size(); }

    // GPU STORAGE ACCESS
    void bind() const;
    void unbind() const;

    // CPU STORAGE ACCESS
    const Vertex3DBuffer& vertexBuffer() const { return *d_vertexData.get(); }
    const IndexBuffer& indexBuffer() const { return *d_indexData.get(); }

    bool operator==(const Model3D& other) const;
};

}