#include "3D/Shader.h"
#include "Utility/Log.h"
#include "Utility/Maths.h"

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

namespace Sprocket {

namespace {

// Parses a shader source code into a string ready to be compiled.
std::string parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string shader((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	return shader;
}

// Give a name for a uniform that is an array, return the accessor
// name for the given index.
std::string arrayName(const std::string& uniformName, size_t index)
{
	std::stringstream ss;
	ss << uniformName << "[" << index << "]";
	return ss.str();
}

}

Shader::Shader(const std::string& vertShaderFile,
               const std::string& fragShaderFile)
{
    SPKT_LOG_TRACE("Building shader: {}", vertShaderFile);
	std::string vertShader = parseShader(vertShaderFile);

    SPKT_LOG_TRACE("Building shader: {}", fragShaderFile);
	std::string fragShader = parseShader(fragShaderFile);

	createShader(vertShader, fragShader);
}

Shader::~Shader()
{
    unbind();
    glDetachShader(d_programId, d_vertShaderId);
    glDetachShader(d_programId, d_fragShaderId);
    glDeleteShader(d_vertShaderId);
    glDeleteShader(d_fragShaderId);
    glDeleteProgram(d_programId);
}

void Shader::createShader(const std::string& vertShader,
                          const std::string& fragShader)
{
	d_programId = glCreateProgram();

	d_vertShaderId = compileShader(GL_VERTEX_SHADER, vertShader);
	d_fragShaderId = compileShader(GL_FRAGMENT_SHADER, fragShader);

	glAttachShader(d_programId, d_vertShaderId);
	glAttachShader(d_programId, d_fragShaderId);
	glLinkProgram(d_programId);
	glValidateProgram(d_programId);
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
        SPKT_LOG_ERROR("Could not compile shader {}", (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::bind() const
{
    glUseProgram(d_programId);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::loadCamera(const Camera& camera) const
{
	Maths::mat4 view = Maths::createViewMatrix(
        camera.position(),
        camera.pitch(),
        camera.yaw(),
        camera.roll()
    );
    loadMatrix4f("viewMatrix", view);
}

void Shader::loadEntity(const Entity& entity) const
{
	// Entity Position
	Maths::mat4 transform = Maths::createTransformationMatrix(
		entity.position(),
		entity.rotation(),
		entity.scale());
    loadMatrix4f("transformMatrix", transform);

	// Specular Lighting (based on Texture)
	loadFloat("shineDamper", entity.texture().shineDamper());
	loadFloat("reflectivity", entity.texture().reflectivity());
}

void Shader::loadLights(const std::vector<Light>& lights) const
{
	for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < lights.size()) {
			loadVector3f(arrayName("lightPositions", i), lights[i].position);
			loadVector3f(arrayName("lightColours", i), lights[i].colour);
			loadVector3f(arrayName("lightAttenuations", i), lights[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			loadVector3f(arrayName("lightPositions", i), {0.0f, 0.0f, 0.0f});
			loadVector3f(arrayName("lightColours", i), {0.0f, 0.0f, 0.0f});
			loadVector3f(arrayName("lightAttenuations", i), {1.0f, 0.0f, 0.0f});
		}
	}
	
}

void Shader::loadProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane) const
{
	Maths::mat4 matrix = Sprocket::Maths::createProjectionMatrix(
        aspectRatio,
		fov,
		nearPlane,
		farPlane);
	bind();
	loadMatrix4f("projectionMatrix", matrix);
	unbind();
}

unsigned int Shader::getUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(d_programId, name.c_str());
}

void Shader::loadFloat(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::loadVector3f(const std::string& name, const Maths::vec3& vector) const
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::loadMatrix4f(const std::string& name, const Maths::mat4& matrix) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

}