#pragma once
#include "Log.h"
#include "Types.h"

#include <memory>

namespace Sprocket {

enum class ResourceType { VAO, VBO };

u32 GenerateBuffer(ResourceType type);
void DeleteBuffer(ResourceType type, u32 buf);

template <ResourceType Type>
class Resource
{
    u32 d_id;

public:
    Resource();
    ~Resource();

    u32 Value() const { return d_id; }

private:
    // Resources can not be copied.
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;

    // Resource can not be moved.
    Resource(Resource&&) = delete;
    Resource& operator=(Resource&&) = delete;
};

using VAO = Resource<ResourceType::VAO>;
using VBO = Resource<ResourceType::VBO>;

template <ResourceType Type>
Resource<Type>::Resource()
    : d_id(GenerateBuffer(Type))
{
}

template <ResourceType Type>
Resource<Type>::~Resource()
{
    DeleteBuffer(Type, d_id);
}

}