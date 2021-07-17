#pragma once
#include <cstddef>
#include <utility>
#include <vector>

namespace spkt {

enum class DataType { FLOAT, INT, UBYTE };
enum class DataRate { VERTEX, INSTANCE };

struct LayoutAttribute
{
    DataType      type;
    std::uint32_t count;
    DataRate      rate;
};

class BufferLayout
{
    std::uint32_t d_vertexSize;                       
    std::uint32_t d_startingIndex;
    std::uint32_t d_currentSize;

    std::vector<LayoutAttribute> d_attributes;

public:
    BufferLayout(std::uint32_t vertexSize, std::uint32_t startingIndex = 0);

    // Append an attribute to the end of the layout.
    void AddAttribute(DataType type, std::uint32_t count, DataRate rate = DataRate::VERTEX);

    // Returns true if the sum of the attributes equals the
    // vertex size and false otherwise.
    bool Validate() const;

    // Sets the attribute pointers of the currently bound vertex array
    // and vertex buffer to match this BufferLayout.
    void SetAttributes() const;

    // Returns the starting index of this buffer layout.
    std::uint32_t GetStartingIndex() const;

    // Returns the vector describing this layout.
    std::vector<LayoutAttribute> GetLayout() const;
};
    
}