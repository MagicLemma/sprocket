#pragma once
#include "Types.h"

#include <vector>
#include <utility>

namespace spkt {

enum class DataType { FLOAT, INT, UBYTE };
enum class DataRate { VERTEX, INSTANCE };

struct LayoutAttribute
{
    DataType type;
    u32      count;
    DataRate rate;
};

class BufferLayout
{
    u32 d_vertexSize;                       
    u32 d_startingIndex;
    u32 d_currentSize;

    std::vector<LayoutAttribute> d_attributes;

public:
    BufferLayout(u32 vertexSize, u32 startingIndex = 0);

    // Append an attribute to the end of the layout.
    void AddAttribute(DataType type, u32 count, DataRate rate = DataRate::VERTEX);

    // Returns true if the sum of the attributes equals the
    // vertex size and false otherwise.
    bool Validate() const;

    // Sets the attribute pointers of the currently bound vertex array
    // and vertex buffer to match this BufferLayout.
    void SetAttributes() const;

    // Returns the starting index of this buffer layout.
    u32 GetStartingIndex() const;

    // Returns the vector describing this layout.
    std::vector<LayoutAttribute> GetLayout() const;
};
    
}