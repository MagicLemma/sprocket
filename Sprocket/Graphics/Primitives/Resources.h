#pragma once
#include "Log.h"

#include <memory>

namespace Sprocket {

enum class ResourceType
{
    VAO,
    VBO,
    Texture,
    FrameBuffer,
    RenderBuffer
};

unsigned int generateBuffer(ResourceType type);
void deleteBuffer(ResourceType type, unsigned int buf);

template <ResourceType Type>
class Resource
{
    unsigned int d_id;

public:
    Resource();
    ~Resource();

    unsigned int value() const { return d_id; }

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
using FBO = Resource<ResourceType::FrameBuffer>;
using RBO = Resource<ResourceType::RenderBuffer>;

template <ResourceType Type>
Resource<Type>::Resource()
    : d_id(generateBuffer(Type))
{
}

template <ResourceType Type>
Resource<Type>::~Resource()
{
    deleteBuffer(Type, d_id);
}

}