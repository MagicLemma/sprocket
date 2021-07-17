#include "BufferLayout.h"

#include <glad/glad.h>

#include <cstddef>

namespace spkt {
namespace {

constexpr int getType(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return GL_FLOAT;
        case DataType::INT: return GL_INT;
        case DataType::UBYTE: return GL_UNSIGNED_BYTE;
    }
}

constexpr int getNormalised(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return GL_FALSE;
        case DataType::INT: return GL_FALSE;
        case DataType::UBYTE: return GL_TRUE;
    }
}

constexpr std::uint32_t getSize(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return sizeof(float);
        case DataType::INT: return sizeof(int);
        case DataType::UBYTE: return sizeof(unsigned char);
    }
}

}

BufferLayout::BufferLayout(std::uint32_t vertexSize, std::uint32_t startingIndex)
    : d_vertexSize(vertexSize)
    , d_startingIndex(startingIndex)
    , d_currentSize(0)
{   
}

void BufferLayout::AddAttribute(DataType type, std::uint32_t count, DataRate rate)
{
    d_attributes.push_back({type, count, rate});
    d_currentSize += count * getSize(type);
}

bool BufferLayout::Validate() const
{
    return d_currentSize == d_vertexSize;
}

void BufferLayout::SetAttributes() const
{
    std::uint32_t offset = 0;
    std::uint32_t index = d_startingIndex;
    for (const auto& data : d_attributes) {
        glEnableVertexAttribArray(index);
        if (data.type == DataType::INT) {
            glVertexAttribIPointer(
                index,
                data.count,
                getType(data.type),
                d_vertexSize,
                (void*)offset
            );
        } else {
            glVertexAttribPointer(
                index,
                data.count,
                getType(data.type),
                getNormalised(data.type),
                d_vertexSize,
                (void*)offset
            );
        }
        switch (data.rate) {
            case DataRate::INSTANCE: {
                glVertexAttribDivisor(index, 1);
            } break;
            case DataRate::VERTEX: {
                glVertexAttribDivisor(index, 0);
            } break;
        }
        offset += data.count * getSize(data.type);
        ++index;
    }
    
}
    
}