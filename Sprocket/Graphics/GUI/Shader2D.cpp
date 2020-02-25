#include "Graphics/GUI/Shader2D.h"
#include "Utility/Log.h"

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

#include <string>

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

Shader2D::Shader2D(const std::string& vertShaderFile,
               const std::string& fragShaderFile)
{
    SPKT_LOG_TRACE("Building shader: {}", vertShaderFile);
	std::string vertShader = parseShader(vertShaderFile);

    SPKT_LOG_TRACE("Building shader: {}", fragShaderFile);
	std::string fragShader = parseShader(fragShaderFile);

	createShader(vertShader, fragShader);
}

Shader2D::~Shader2D()
{
    unbind();
    glDetachShader(d_programId, d_vertShaderId);
    glDetachShader(d_programId, d_fragShaderId);
    glDeleteShader(d_vertShaderId);
    glDeleteShader(d_fragShaderId);
    glDeleteProgram(d_programId);
}

void Shader2D::createShader(const std::string& vertShader,
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

unsigned int Shader2D::compileShader(unsigned int type, const std::string& source)
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

void Shader2D::bind() const
{
    glUseProgram(d_programId);
}

void Shader2D::unbind() const
{
    glUseProgram(0);
}

}