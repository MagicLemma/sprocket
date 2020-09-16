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
};

enum class BufferUsage { STATIC, DYNAMIC };

class Buffer
{
    std::shared_ptr<VBO> d_buffer;

    BufferLayout d_layout;
    BufferUsage  d_usage;
    
    std::size_t d_instanceCount;
    std::size_t d_instanceSize;

public:
    Buffer(
        const BufferLayout& layout,
        const BufferUsage& usage
    );

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
void Buffer::SetData(const std::vector<T>& data)
{
    SetData(data.size(), sizeof(T), data.data());
}

}