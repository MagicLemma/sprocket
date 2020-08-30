#pragma once
#include "Maths.h"
#include "Resources.h"

#include <vector>

namespace Sprocket {

struct InstanceData
{
    Maths::vec3 position;
    Maths::quat orientation;
    Maths::vec3 scale;
    float       shineDamper;
    float       reflectivity;
};

class InstanceBuffer
{
    std::shared_ptr<VBO> d_buffer;

    std::vector<InstanceData> d_data;

public:
    InstanceBuffer(std::size_t initialSize = 1000);

    void Bind();
    void Clear();

    void Add(const InstanceData& data);

    std::size_t Size() const { return d_data.size(); }
};

}