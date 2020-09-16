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
std::string ParseShader(const std::string& filepath)
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
	std::string vertShader = ParseShader(vertShaderFile);
	std::string fragShader = ParseShader(fragShaderFile);
	
	this->d_vertexSource = vertShader;
	this->d_fragSource = fragShader;
	
	CreateShader(vertShader, fragShader);
}

Shader::~Shader()
{
    Unbind();
    glDetachShader(d_programId, d_vertShaderId);
    glDetachShader(d_programId, d_fragShaderId);
    glDeleteShader(d_vertShaderId);
    glDeleteShader(d_fragShaderId);
    glDeleteProgram(d_programId);
}

void Shader::CreateShader(const std::string& vertShader,
                          const std::string& fragShader)
{
	d_programId = glCreateProgram();

	d_vertShaderId = CompileShader(GL_VERTEX_SHADER, vertShader);
	d_fragShaderId = CompileShader(GL_FRAGMENT_SHADER, fragShader);

	glAttachShader(d_programId, d_vertShaderId);
	glAttachShader(d_programId, d_fragShaderId);
	glLinkProgram(d_programId);
	glValidateProgram(d_programId);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

void Shader::Bind() const
{
    glUseProgram(d_programId);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
	return glGetUniformLocation(d_programId, name.c_str());
}

void Shader::LoadUniformFloat(const std::string& name, float value) const
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::LoadUniformSampler(const std::string& name, int value) const
{
	glProgramUniform1i(d_programId, GetUniformLocation(name), value);
}

void Shader::LoadUniformInt(const std::string& name, int value) const
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::LoadUniformVec2(const std::string& name, const Maths::vec2& vector) const
{
	glUniform2f(GetUniformLocation(name), vector.x, vector.y);
}

void Shader::LoadUniformVec3(const std::string& name, const Maths::vec3& vector) const
{
	glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::LoadUniformVec4(const std::string& name, const Maths::vec4& vector) const
{
	glUniform4f(GetUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::LoadUniformQuat(const std::string& name, const Maths::quat& quat) const
{
	glUniform4f(GetUniformLocation(name), quat.x, quat.y, quat.z, quat.w);
}

void Shader::LoadUniformMat4(const std::string& name, const Maths::mat4& matrix) const
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

std::string ArrayName(const std::string& uniformName, size_t index)
{
	std::stringstream ss;
	ss << uniformName << "[" << index << "]";
	return ss.str();
}

bool Shader::Reload()
{
	unsigned int programId = glCreateProgram();

	unsigned int vertShaderId = CompileShader(GL_VERTEX_SHADER, d_vertexSource);

	if(vertShaderId == 0) {
		glDeleteProgram(programId);

		return false;
	}

	unsigned int fragShaderId = CompileShader(GL_FRAGMENT_SHADER, d_fragSource);

	if(fragShaderId == 0) {
		glDeleteShader(vertShaderId);
		glDeleteProgram(programId);

		return false;
	}

	glAttachShader(programId, vertShaderId);
	glAttachShader(programId, fragShaderId);
	glLinkProgram(programId);
	glValidateProgram(programId);

	GLint validateStatus = GL_FALSE;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &validateStatus);

	if(validateStatus != GL_TRUE) {
		glDeleteShader(fragShaderId);
		glDeleteShader(vertShaderId);
		glDeleteProgram(programId);
		return false;
	}

	glDeleteShader(d_vertShaderId);
	glDeleteShader(d_fragShaderId);
	glDeleteProgram(d_programId);

	d_programId = programId;
	d_vertShaderId = vertShaderId;
	d_fragShaderId = fragShaderId;

	return true;
}

}