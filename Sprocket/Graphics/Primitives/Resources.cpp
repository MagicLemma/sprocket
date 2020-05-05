#include "Resources.h"
#include "Log.h"

#include <exception>

#include <glad/glad.h>

namespace Sprocket {

unsigned int generateBuffer(ResourceType type)
{
    unsigned int buf;
    switch(type) {
        case ResourceType::VAO: {
            glGenVertexArrays(1, &buf);
        } break;
        case ResourceType::VBO: {
            glGenBuffers(1, &buf);
        } break;
        case ResourceType::Texture: {
            glGenTextures(1, &buf);
        } break;
        case ResourceType::FrameBuffer: {
            glGenFramebuffers(1, &buf);
        } break;
        case ResourceType::RenderBuffer: {
            glGenRenderbuffers(1, &buf);
        } break;
        default: {
            SPKT_LOG_FATAL("Unknown Resource type!");
            throw std::runtime_error("Unknown Resource type!");
        }
    }
    return buf;
}

void deleteBuffer(ResourceType type, unsigned int buf)
{
    switch(type) {
        case ResourceType::VAO: {
            SPKT_LOG_INFO("Cleaning up VAO");
            glDeleteVertexArrays(1, &buf);
        } break;
        case ResourceType::VBO: {
            glDeleteBuffers(1, &buf);
        } break;
        case ResourceType::Texture: {
            glDeleteTextures(1, &buf);
        } break;
        case ResourceType::FrameBuffer: {
            glDeleteFramebuffers(1, &buf);
        } break;
        case ResourceType::RenderBuffer: {
            glDeleteRenderbuffers(1, &buf);
        } break;
        default: {
            SPKT_LOG_FATAL("Unknown Resource type!");
        }
    }
}
    
}