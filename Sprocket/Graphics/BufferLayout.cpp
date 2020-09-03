#include "BufferLayout.h"
#include "Log.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

int getType(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return GL_FLOAT;
        case DataType::UBYTE: return GL_UNSIGNED_BYTE;
    }
    return GL_FLOAT;
}

int getNormalised(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return GL_FALSE;
        case DataType::UBYTE: return GL_TRUE;
    }
    return GL_FALSE;
}

unsigned int getSize(DataType type)
{
    switch (type) {
        case DataType::FLOAT: return sizeof(float);
        case DataType::UBYTE: return sizeof(unsigned char);
    }
    return sizeof(float);
}

}

BufferLayout::BufferLayout(unsigned int vertexSize, unsigned int startingIndex)
    : d_vertexSize(vertexSize)
    , d_startingIndex(startingIndex)
    , d_currentSize(0)
{   
}

void BufferLayout::AddAttribute(DataType type, unsigned int count, DataRate rate)
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
    unsigned int offset = 0;
    unsigned int index = d_startingIndex;
    for (const auto& data : d_attributes) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            data.count,
            getType(data.type),
            getNormalised(data.type),
            d_vertexSize,
            (void*)offset
        );
        if (data.rate == DataRate::INSTANCE) {
            glVertexAttribDivisor(index, 1);
        }
        offset += data.count * getSize(data.type);
        ++index;
    }
    
}
    
}