#pragma once
#include <Sprocket/Graphics/BufferLayout.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace spkt {

struct InstanceData
{
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

    static void set_buffer_attributes();
};

enum class BufferUsage { STATIC, DYNAMIC };

class Buffer
{
    std::uint32_t d_vbo;

    spkt::BufferLayout d_layout;
    BufferUsage  d_usage;
    
    std::size_t d_instanceCount;
    std::size_t d_instanceSize;

    void SetData(std::size_t count, std::size_t size, const void* data);
    
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

public:
    Buffer(const BufferLayout& layout, const BufferUsage& usage);
    // TODO Add destructor to clean up GL resources

    void Bind() const;
    std::size_t Size() const { return d_instanceCount; }

    template <typename T> void SetData(const std::vector<T>& data);
};

template <typename T>
void Buffer::SetData(const std::vector<T>& data)
{
    SetData(data.size(), sizeof(T), data.data());
}

namespace detail {

std::uint32_t new_vbo();
void delete_vbo(std::uint32_t vbo);
void bind_vbo(std::uint32_t vbo);
void unbind_vbo(std::uint32_t vbo);
void set_data(std::uint32_t vbo, std::size_t size, const void* data, BufferUsage usage);

}

template <typename T>
concept buffer_element = requires
{
    { T::set_buffer_attributes() } -> std::same_as<void>;
};

template <buffer_element T, BufferUsage Usage = BufferUsage::STATIC>
class buffer
{
    std::uint32_t d_vbo;
    std::size_t   d_size;

    buffer(const buffer&) = delete;
    buffer& operator=(const buffer&) = delete;

public:
    buffer() : d_vbo(detail::new_vbo()), d_size(0) {}
    ~buffer() { detail::delete_vbo(d_vbo); }

    void bind() const
    {
        detail::bind_vbo(d_vbo);
        T::set_buffer_attributes();
        detail::unbind_vbo(d_vbo);
    }

    void set_data(std::span<const T> data)
    {
        d_size = data.size();
        detail::set_data(d_vbo, data.size_bytes(), data.data(), Usage);
    }

    std::size_t size() const { return d_size; }
};

}