#pragma once
#include "Light.h"

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

    void bind() const;
    void unbind() const;

    void loadFloat(const std::string& name, float value) const;
    void loadVector3f(const std::string& name, const glm::vec3& vector) const;
    void loadMatrix4f(const std::string& name, const glm::mat4& matrix) const;

    void loadProjectionMatrix(float aspectRatio,
                              float fov,
                              float nearPlane,
                              float farPlane) const;

    void loadLight(const Light& light);
};

}