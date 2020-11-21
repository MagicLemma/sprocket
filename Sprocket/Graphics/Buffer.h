#pragma once
#include "Maths.h"
#include "Types.h"
#include "BufferLayout.h"
#include "Resources.h"

#include <vector>

namespace Sprocket {

struct InstanceData
{
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;
};

enum class BufferUsage { STATIC, DYNAMIC };

class Buffer
{
    u32 d_vbo;

    BufferLayout d_layout;
    BufferUsage  d_usage;
    
    std::size_t d_instanceCount;
    std::size_t d_instanceSize;

    void SetData(std::size_t count, std::size_t size, const void* data);
    
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public:
    Buffer(const BufferLayout& layout, const BufferUsage& usage);

    void Bind() const;
    std::size_t Size() const { return d_instanceCount; }

    template <typename T> void SetData(const std::vector<T>& data);
};

template <typename T>
void Buffer::SetData(const std::vector<T>& data)
{
    SetData(data.size(), sizeof(T), data.data());
}

}