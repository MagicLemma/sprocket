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

    // Shader Creation
    void createShader(const std::string& vertShader,
                      const std::string& fragShader);

    unsigned int compileShader(unsigned int type, const std::string& source);

    // Shader Uniform Getter
    unsigned int getUniformLocation(const std::string& name) const;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    void bind() const;
    void unbind() const;

    // Shader Uniform Setters
    void loadUniformInt(const std::string& name, int value) const;
    void loadUniform(const std::string& name, float value) const;
    void loadUniform(const std::string& name, const Maths::vec2& vector) const;
    void loadUniform(const std::string& name, const Maths::vec3& vector) const;
    void loadUniform(const std::string& name, const Maths::vec4& vector) const;
    void loadUniform(const std::string& name, const Maths::mat4& matrix) const;
};

// HELPER FUNCTIONS
std::string arrayName(const std::string& uniformName, size_t index);
    // Give a name for a uniform that is an array, return the accessor
    // name for the given index.

}