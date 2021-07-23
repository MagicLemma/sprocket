#pragma once
#include <glm/glm.hpp>

#include <memory>
#include <string_view>
#include <string>
#include <vector>

namespace spkt {

class Shader
{
    // Shader IDs
    std::uint32_t d_programId;
    std::uint32_t d_vertShaderId;
    std::uint32_t d_fragShaderId;

    std::string d_vertexSource;
    std::string d_fragSource;

    // Shader Creation
    void CreateShader(const std::string& vertShader,
                      const std::string& fragShader);

    std::uint32_t CompileShader(std::uint32_t type, const std::string& source);

    // Shader Uniform Getter
    std::uint32_t GetUniformLocation(const std::string& name) const;

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    bool Reload();
    
    std::string& VertexShaderSource() { return d_vertexSource; }
    std::string& FragShaderSource() { return d_fragSource; }

    std::uint32_t Id() const { return d_programId; }

    void Bind() const;
    void Unbind() const;

    // Shader Uniform Setters
    void LoadSampler(const std::string& name, int value) const;
    void LoadInt(const std::string& name, int value) const;
    void LoadFloat(const std::string& name, float value) const;
    void LoadVec2(const std::string& name, const glm::vec2& vector) const;
    void LoadVec3(const std::string& name, const glm::vec3& vector) const;
    void LoadVec4(const std::string& name, const glm::vec4& vector) const;
    void LoadQuat(const std::string& name, const glm::quat& quat) const;
    void LoadMat4(const std::string& name, const glm::mat4& matrix, int count = 1) const;
};

using shader_ptr = std::unique_ptr<Shader>;

// HELPER FUNCTIONS
std::string ArrayName(std::string_view uniformName, std::size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}