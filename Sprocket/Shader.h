#pragma once
#include "Light.h"
#include "Camera.h"
#include "Entity.h"

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Sprocket {

class Shader
{
    // Constants
    static constexpr int MAX_NUM_LIGHTS = 5;

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

    // Shader Uniform Setters
    void loadFloat(const std::string& name, float value) const;
    void loadVector3f(const std::string& name, const glm::vec3& vector) const;
    void loadMatrix4f(const std::string& name, const glm::mat4& matrix) const;

public:
    Shader(const std::string& vertShaderFile,
           const std::string& fragShaderFile);

    ~Shader();

    void bind() const;
    void unbind() const;

    // Load Object Data
    void loadCamera(const Camera& camera) const;
    void loadEntity(const Entity& entity) const;
    void loadLights(const std::vector<Light>& lights) const;

    void loadProjectionMatrix(float aspectRatio,
                              float fov,
                              float nearPlane,
                              float farPlane) const;


};

}