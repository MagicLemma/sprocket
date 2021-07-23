#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace spkt {

class Shader
{
    std::string d_vertexSource;
    std::string d_fragSource;

    // Shader IDs
    std::uint32_t d_programId;
    std::uint32_t d_vertShaderId;
    std::uint32_t d_fragShaderId;

    // Shader Uniform Getter
    std::uint32_t uniform_location(const std::string& name) const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

public:
    Shader(std::string_view vertShaderFile, std::string_view fragShaderFile);
    ~Shader();

    bool Reload();
    
    std::string& vertex_source() { return d_vertexSource; }
    std::string& fragment_source() { return d_fragSource; }

    void Bind() const;
    void Unbind() const;

    // Shader Uniform Setters
    void load(const std::string& name, int value) const;
    void load(const std::string& name, float value) const;
    void load(const std::string& name, const glm::vec2& vector) const;
    void load(const std::string& name, const glm::vec3& vector) const;
    void load(const std::string& name, const glm::vec4& vector) const;
    void load(const std::string& name, const glm::quat& quat) const;
    void load(const std::string& name, const glm::mat4& matrix, int count = 1) const;
};

using shader_ptr = std::unique_ptr<Shader>;

// HELPER FUNCTIONS
std::string ArrayName(std::string_view uniformName, std::size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}