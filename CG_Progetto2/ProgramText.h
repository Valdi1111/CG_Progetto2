#pragma once
#include "Program.h"
#include "TextCharacter.h"
#include <map>

class ProgramText : public Program
{
private:
	GLuint projectionId;
	map<GLchar, TextCharacter> charactersMap;
	GLuint textColorId;
	unsigned int vao;
	unsigned int vbo;
public:
	ProgramText() {}

	GLuint getProjectionId() { return this->projectionId; }
	GLuint getTextColorId() { return this->textColorId; }
	void initFreeType();
	void initUniforms();
	void initVao();
	void switchProgram(mat4 projection);
	void renderText(string text, float x, float y, float scale, vec3 color);
};

