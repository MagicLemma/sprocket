#pragma once
#include <string>

namespace Sprocket {

class Shader
{
    unsigned int d_programId;
    unsigned int d_vertShaderId;
    unsigned int d_fragShaderId;

    void createShader(const std::string& vertShader,
                      const std::string& fragShader);

    unsigned int compileShader(unsigned int type, const std::string& source);

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    void start();
    void stop();

    void bindAttribute(unsigned int attribute, const std::string& name);

};

}