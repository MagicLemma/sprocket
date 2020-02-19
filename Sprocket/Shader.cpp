#include "Shader.h"
#include "Log.h"

#include <fstream>
#include <string>

#include <glad/glad.h>

namespace Sprocket {

namespace {

std::string parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	std::string shader((std::istreambuf_iterator<char>(stream)),
		std::istreambuf_iterator<char>());
	return shader;
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
    stop();
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
        SPKT_LOG_ERROR("Could not compile shader");
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::start()
{
    glUseProgram(d_programId);
}

void Shader::stop()
{
    glUseProgram(0);
}

unsigned int Shader::getUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(d_programId, name.c_str());
}

void Shader::loadFloat(const std::string& name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::loadVector3f(const std::string& name, const glm::vec3& vector)
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::loadMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


}