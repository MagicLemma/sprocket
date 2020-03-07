#include "Graphics/Shader.h"
#include "Utility/Log.h"
#include "Utility/Maths.h"

#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include <glad/glad.h>

namespace Sprocket {

namespace {

// Parses a shader source code into a string ready to be compiled.
std::string parseShader(const std::string& filepath)
{
	if (!std::filesystem::exists(filepath)) {
		SPKT_LOG_FATAL("Shader file '{}' does not exist!", filepath);
	}
	std::ifstream stream(filepath);
	std::string shader((std::istreambuf_iterator<char>(stream)),
		                std::istreambuf_iterator<char>());
	return shader;
}

}

Shader::Shader(const std::string& vertShaderFile,
               const std::string& fragShaderFile)
{
	std::string vertShader = parseShader(vertShaderFile);
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

unsigned int Shader::getUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(d_programId, name.c_str());
}

void Shader::loadUniform(const std::string& name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::loadUniform(const std::string& name, const Maths::vec2& vector) const
{
	glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::loadUniform(const std::string& name, const Maths::vec3& vector) const
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::loadUniform(const std::string& name, const Maths::vec4& vector) const
{
	glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}


void Shader::loadUniform(const std::string& name, const Maths::mat4& matrix) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

std::string arrayName(const std::string& uniformName, size_t index)
{
	std::stringstream ss;
	ss << uniformName << "[" << index << "]";
	return ss.str();
}

}