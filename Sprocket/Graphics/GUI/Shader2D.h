#pragma once

#include <string>

namespace Sprocket {

class Shader2D
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
    Shader2D(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader2D();

    void bind() const;
    void unbind() const;


};


}