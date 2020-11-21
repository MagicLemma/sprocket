#pragma once
#include "Maths.h"
#include "Types.h"

#include <string>
#include <vector>
#include <sstream>

namespace Sprocket {

class Shader
{
    // Shader IDs
    u32 d_programId;
    u32 d_vertShaderId;
    u32 d_fragShaderId;

    std::string d_vertexSource;
    std::string d_fragSource;

    // Shader Creation
    void CreateShader(const std::string& vertShader,
                      const std::string& fragShader);

    u32 CompileShader(u32 type, const std::string& source);

    // Shader Uniform Getter
    u32 GetUniformLocation(const std::string& name) const;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    bool Reload();
    
    std::string& VertexShaderSource() { return d_vertexSource; }
    std::string& FragShaderSource() { return d_fragSource; }

    u32 Id() const { return d_programId; }

    void Bind() const;
    void Unbind() const;

    // Shader Uniform Setters
    void LoadSampler(const std::string& name, int value) const;
    void LoadInt(const std::string& name, int value) const;
    void LoadFloat(const std::string& name, float value) const;
    void LoadVec2(const std::string& name, const glm::vec2& vector) const;
    void LoadVec3(const std::string& name, const Maths::vec3& vector) const;
    void LoadVec4(const std::string& name, const Maths::vec4& vector) const;
    void LoadQuat(const std::string& name, const Maths::quat& quat) const;
    void LoadMat4(const std::string& name, const Maths::mat4& matrix, int count = 1) const;
};

// HELPER FUNCTIONS
std::string ArrayName(const std::string& uniformName, size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}