#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Program
{
protected:
	GLuint programId;
public:
	Program() {}

	GLuint getProgramId() { return this->programId; }
	void initProgram(string vertexFilename, string fragmentFilename);
	void switchProgram();
};

