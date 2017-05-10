#include "Shader.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

Shader::Shader (GLenum shaderType, const char * filename) : mShaderType(shaderType), mHandle(glCreateShader(shaderType)) {

	std::ifstream file(filename);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();

	std::string source = buffer.str();
	const char * sourcePtr = source.c_str();

	glShaderSource(mHandle, 1, &sourcePtr, 0);
	glCompileShader(mHandle);

	GLint logLength;
	glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0) {
		std::vector<char> msg(logLength + 1);
		glGetShaderInfoLog(mHandle, logLength, 0, msg.data());
		fprintf(stderr, "%s\n", msg.data());
		throw std::runtime_error("Shader could not be compiled");
	}
}

Shader::~Shader () {
	glDeleteShader(mHandle);
}

GLenum Shader::getShaderType () {
	return mShaderType;
}

GLuint Shader::getHandle () {
	return mHandle;
}