#include "Program.h"

Program::Program () : mHandle(glCreateProgram()) {
}

Program::~Program () {
	glDeleteProgram(mHandle);
}

void Program::attach (Shader shader) {
	glAttachShader(mHandle, shader.getHandle());
}

void Program::detach (Shader shader) {
	glDetachShader(mHandle, shader.getHandle());
}

void Program::link () {
	glLinkProgram(mHandle);
}

void Program::use () {
	glUseProgram(mHandle);
}

GLuint Program::getUniformLocation (const char * name) {
	return glGetUniformLocation(mHandle, name);
}