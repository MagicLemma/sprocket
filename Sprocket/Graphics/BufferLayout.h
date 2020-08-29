#pragma once
#include <vector>
#include <utility>

namespace Sprocket {

enum class DataType { FLOAT, UBYTE };

class BufferLayout
{
    using DataPair = std::pair<DataType, unsigned int>;

    const unsigned int    d_vertexSize;                       
    unsigned int          d_currentSize;
    std::vector<DataPair> d_dataTypes;

public:
    BufferLayout(unsigned int vertexSize);

    void AddAttribute(DataType type, unsigned int count);
        // Append an attribute to the end of the layout.

    bool Validate() const;
        // Returns true if the sum of the attributes equals the
        // vertex size and false otherwise.

    void SetAttributes() const;
        // Sets the attribute pointers of the currently bound VAO
        // and VBO to match this BufferLayout.
};
    
}