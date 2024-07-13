#pragma once
#include "Lib.h"

class ShaderMaker
{
public:
	static char* readSource(string shaderFile);
	static GLuint initFromFile(string shaderFilename, GLenum type);
	static GLuint createProgram(string vertexfilename, string fragmentfilename);

private:
	ShaderMaker() {}
};