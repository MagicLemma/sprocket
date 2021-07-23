#include "Shader.h"

#include <Sprocket/Utility/Log.h>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <format>
#include <fstream>
#include <string>

namespace spkt {

namespace {

// Parses a shader source code into a string ready to be compiled.
std::string parse_shader_source(std::string_view filepath)
{
	std::ifstream stream(std::string{filepath});
	if (!stream) {
		log::fatal("Shader file '{}' does not exist!", filepath);
	}
	std::string shader((std::istreambuf_iterator<char>(stream)),
		                std::istreambuf_iterator<char>());
	return shader;
}

std::uint32_t compile_shader_source(std::uint32_t type, const std::string& source)
{
	std::uint32_t id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
        log::error("Could not compile shader {}", (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
		glDeleteShader(id);
		return 0;
	}

	return id;
}

}

Shader::Shader(
	std::string_view vertShaderFile,
	std::string_view fragShaderFile
)
	: d_vertexSource(parse_shader_source(vertShaderFile))
	, d_fragSource(parse_shader_source(fragShaderFile))
	, d_programId(glCreateProgram())
	, d_vertShaderId(compile_shader_source(GL_VERTEX_SHADER, d_vertexSource))
	, d_fragShaderId(compile_shader_source(GL_FRAGMENT_SHADER, d_fragSource))
{
	glAttachShader(d_programId, d_vertShaderId);
	glAttachShader(d_programId, d_fragShaderId);
	glLinkProgram(d_programId);
	glValidateProgram(d_programId);
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

void Shader::Bind() const
{
    glUseProgram(d_programId);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

std::uint32_t Shader::uniform_location(const std::string& name) const
{
	return glGetUniformLocation(d_programId, name.c_str());
}

void Shader::LoadFloat(const std::string& name, float value) const
{
	glUniform1f(uniform_location(name), value);
}

void Shader::LoadSampler(const std::string& name, int value) const
{
	glProgramUniform1i(d_programId, uniform_location(name), value);
}

void Shader::LoadInt(const std::string& name, int value) const
{
	glUniform1i(uniform_location(name), value);
}

void Shader::LoadVec2(const std::string& name, const glm::vec2& vector) const
{
	glUniform2f(uniform_location(name), vector.x, vector.y);
}

void Shader::LoadVec3(const std::string& name, const glm::vec3& vector) const
{
	glUniform3f(uniform_location(name), vector.x, vector.y, vector.z);
}

void Shader::LoadVec4(const std::string& name, const glm::vec4& vector) const
{
	glUniform4f(uniform_location(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::LoadQuat(const std::string& name, const glm::quat& quat) const
{
	glUniform4f(uniform_location(name), quat.x, quat.y, quat.z, quat.w);
}

void Shader::LoadMat4(const std::string& name, const glm::mat4& matrix, int count) const
{
	glUniformMatrix4fv(uniform_location(name), count, GL_FALSE, glm::value_ptr(matrix));
}

std::string ArrayName(std::string_view uniformName, std::size_t index)
{
	return std::format("{}[{}]", uniformName, index);
}

bool Shader::Reload()
{
	std::uint32_t programId = glCreateProgram();

	std::uint32_t vertShaderId = compile_shader_source(GL_VERTEX_SHADER, d_vertexSource);
	if(vertShaderId == 0) {
		glDeleteProgram(programId);
		return false;
	}

	std::uint32_t fragShaderId = compile_shader_source(GL_FRAGMENT_SHADER, d_fragSource);
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