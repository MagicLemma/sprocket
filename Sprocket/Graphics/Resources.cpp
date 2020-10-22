#include "Resources.h"
#include "Log.h"
#include "Types.h"

#include <exception>

#include <glad/glad.h>

namespace Sprocket {

u32 GenerateBuffer(ResourceType type)
{
    u32 buf;
    switch(type) {
        case ResourceType::VAO: {
            glGenVertexArrays(1, &buf);
        } break;
        case ResourceType::VBO: {
            glGenBuffers(1, &buf);
        } break;
        default: {
            SPKT_LOG_FATAL("Unknown Resource type!");
            throw std::runtime_error("Unknown Resource type!");
        }
    }
    return buf;
}

void DeleteBuffer(ResourceType type, u32 buf)
{
    switch(type) {
        case ResourceType::VAO: {
            glDeleteVertexArrays(1, &buf);
        } break;
        case ResourceType::VBO: {
            glDeleteBuffers(1, &buf);
        } break;
        default: {
            SPKT_LOG_FATAL("Unknown Resource type!");
        }
    }
}
    
}