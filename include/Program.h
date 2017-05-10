#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>

#include "Shader.h"

class Program {

private:

	const GLuint mHandle;

public:

	Program ();
	~Program ();
	
	void attach (Shader shader);
	void detach (Shader shader);

	void link ();
	void use ();

	GLuint getUniformLocation (const char * name);

};

#endif