#pragma once
#include <vector>
#include <utility>

namespace Sprocket {

enum class DataType { FLOAT, INT, UBYTE };
enum class DataRate { VERTEX, INSTANCE };

struct LayoutAttribute
{
    DataType     type;
    unsigned int count;
    DataRate     rate;
};

class BufferLayout
{
    const unsigned int d_vertexSize;                       
    const unsigned int d_startingIndex;

    unsigned int                 d_currentSize;
    std::vector<LayoutAttribute> d_attributes;

public:
    BufferLayout(unsigned int vertexSize, unsigned int startingIndex = 0);

    // Append an attribute to the end of the layout.
    void AddAttribute(DataType type, unsigned int count, DataRate rate = DataRate::VERTEX);

    // Returns true if the sum of the attributes equals the
    // vertex size and false otherwise.
    bool Validate() const;

    // Sets the attribute pointers of the currently bound VAO
    // and VBO to match this BufferLayout.
    void SetAttributes() const;

    // Returns the starting index of this buffer layout.
    unsigned int GetStartingIndex() const;

    // Returns the vector describing this layout.
    std::vector<LayoutAttribute> GetLayout() const;
};
    
}