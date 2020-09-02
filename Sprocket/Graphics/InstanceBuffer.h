#pragma once
#include "Maths.h"
#include "BufferLayout.h"
#include "Resources.h"

#include <vector>

namespace Sprocket {

struct InstanceData
{
    Maths::vec3 position;
    Maths::quat orientation;
    Maths::vec3 scale;
    float       shineDamper;
    float       reflectivity;
};

class InstanceBuffer
{
    std::shared_ptr<VBO> d_buffer;

    BufferLayout d_layout;
    std::size_t d_instanceCount;
    std::size_t d_instanceSize;

public:
    InstanceBuffer(const BufferLayout& layout);

    void Bind();

    void SetData(
        std::size_t instanceCount,
        std::size_t instanceSize,
        const void* data
    );

    template <typename T> void SetData(const std::vector<T>& data);

    std::size_t Size() const { return d_instanceCount; }
};

template <typename T>
void InstanceBuffer::SetData(const std::vector<T>& data)
{
    SetData(data.size(), sizeof(T), data.data());
}

}