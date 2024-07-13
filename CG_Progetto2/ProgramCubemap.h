#pragma once
#include "Program.h"
#include "Strutture.h"

class ProgramCubemap : public Program
{
protected:
	GLuint skyboxId;
	GLuint viewId;
	GLuint projectionId;

public:
	ProgramCubemap() {}

	GLuint getSkyboxId() { return this->skyboxId; }
	GLuint getViewId() { return this->viewId; }
	GLuint getProjectionId() { return this->projectionId; }
	void initUniforms();
	void switchProgram(mat4 Projection, mat4 View);
};

