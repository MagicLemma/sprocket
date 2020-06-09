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

BufferLayout::BufferLayout(unsigned int vertexSize)
    : d_vertexSize(vertexSize)
    , d_currentSize(0)
    , d_dataTypes()
{   
}

void BufferLayout::AddAttribute(DataType type, unsigned int count)
{
    d_dataTypes.push_back(std::make_pair(type, count));
    d_currentSize += count * getSize(type);
}

bool BufferLayout::Validate() const
{
    return d_currentSize == d_vertexSize;
}

void BufferLayout::SetAttributes() const
{
    unsigned int offset = 0;
    unsigned int index = 0;
    for (const auto& data : d_dataTypes) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            data.second,
            getType(data.first),
            getNormalised(data.first),
            d_vertexSize,
            (void*)offset
        );
        offset += data.second * getSize(data.first);
        ++index;
    }
    
}
    
}