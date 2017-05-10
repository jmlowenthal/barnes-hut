#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader {

private:

	const GLuint mHandle;
	const GLenum mShaderType;

public:

	Shader (GLenum shaderType, const char * filename);
	~Shader ();

	GLenum getShaderType ();
	GLuint getHandle ();

};

#endif