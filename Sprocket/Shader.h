#pragma once
#include <string>

#include <glm/glm.hpp>

namespace Sprocket {

class Shader
{
    unsigned int d_programId;
    unsigned int d_vertShaderId;
    unsigned int d_fragShaderId;

    void createShader(const std::string& vertShader,
                      const std::string& fragShader);

    unsigned int compileShader(unsigned int type, const std::string& source);

    unsigned int getUniformLocation(const std::string& name) const;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    void start();
    void stop();

    void loadFloat(const std::string& name, float value);
    void loadVector3f(const std::string& name, const glm::vec3& vector);
    void loadMatrix4f(const std::string& name, const glm::mat4& matrix);

};

}