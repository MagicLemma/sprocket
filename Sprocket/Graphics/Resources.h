#pragma once
#include "Log.h"

#include <memory>

namespace Sprocket {

enum class ResourceType
{
    VAO,
    VBO,
    Texture,
};

unsigned int GenerateBuffer(ResourceType type);
void DeleteBuffer(ResourceType type, unsigned int buf);

template <ResourceType Type>
class Resource
{
    unsigned int d_id;

public:
    Resource();
    ~Resource();

    unsigned int Value() const { return d_id; }

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
using TEX = Resource<ResourceType::Texture>;

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