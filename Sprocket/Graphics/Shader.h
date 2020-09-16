#pragma once
#include "Utility/Maths.h"

#include <string>
#include <vector>
#include <sstream>

namespace Sprocket {

class Shader
{
    // Shader IDs
    unsigned int d_programId;
    unsigned int d_vertShaderId;
    unsigned int d_fragShaderId;

    std::string d_vertexSource;
    std::string d_fragSource;

    // Shader Creation
    void CreateShader(const std::string& vertShader,
                      const std::string& fragShader);

    unsigned int CompileShader(unsigned int type, const std::string& source);

    // Shader Uniform Getter
    unsigned int GetUniformLocation(const std::string& name) const;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    bool Reload();
    
    std::string& VertexShaderSource() { return d_vertexSource; }
    std::string& FragShaderSource() { return d_fragSource; }

    unsigned int Id() const { return d_programId; }

    void Bind() const;
    void Unbind() const;

    // Shader Uniform Setters
    void LoadUniformSampler(const std::string& name, int value) const;
    void LoadUniformInt(const std::string& name, int value) const;
    void LoadUniform(const std::string& name, float value) const;
    void LoadUniform(const std::string& name, const Maths::vec2& vector) const;
    void LoadUniform(const std::string& name, const Maths::vec3& vector) const;
    void LoadUniform(const std::string& name, const Maths::vec4& vector) const;
    void LoadUniform(const std::string& name, const Maths::quat& quat) const;
    void LoadUniform(const std::string& name, const Maths::mat4& matrix) const;
};

// HELPER FUNCTIONS
std::string ArrayName(const std::string& uniformName, size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}