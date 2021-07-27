#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace spkt {

template <typename T>
concept buffer_element = requires
{
    { T::set_buffer_attributes() } -> std::same_as<void>;
};

struct static_vertex
{
    glm::vec3 position;
    glm::vec2 textureCoords;

    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    static void set_buffer_attributes();
};

struct animated_vertex
{
    glm::vec3 position;
    glm::vec2 textureCoords;

    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    glm::ivec4 boneIndices = {-1, -1, -1, -1};
    glm::vec4  boneWeights = {0.0, 0.0, 0.0, 0.0};

    static void set_buffer_attributes();
};

struct ui_vertex
{
    glm::vec2 position;
    glm::vec4 colour;
    glm::vec2 textureCoords = {0.0, 0.0};

    static void set_buffer_attributes();
};

struct model_instance
{
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;

    static void set_buffer_attributes();
};

}