#include "Program.h"
#include "ShaderMaker.h"

void Program::initProgram(string vertexFilename, string fragmentFilename)
{
	GLenum errorCheckValue = glGetError();

	GLuint vertexShader = ShaderMaker::initFromFile(vertexFilename, GL_VERTEX_SHADER);
	GLuint fragmentShader = ShaderMaker::initFromFile(fragmentFilename, GL_FRAGMENT_SHADER);

	errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		fprintf(stderr, "ERROR %d: Could not create the shaders: %s \n", errorCheckValue, gluErrorString(errorCheckValue));
		exit(-1);
	}

	this->programId = glCreateProgram();
	glAttachShader(this->getProgramId(), vertexShader);
	glAttachShader(this->getProgramId(), fragmentShader);
	glLinkProgram(this->getProgramId());
	glValidateProgram(this->getProgramId());
}

void Program::switchProgram()
{
	glUseProgram(this->getProgramId());
}