#pragma once
#include "Program.h"
#include "Strutture.h"

class ProgramRiflessione : public Program
{
protected:
	GLuint modelId;
	GLuint viewId;
	GLuint projectionId;
	GLuint viewPosId;
	GLuint cubemapId;

public:
	ProgramRiflessione() {}

	GLuint getModelId() { return this->modelId; }
	GLuint getViewId() { return this->viewId; }
	GLuint getProjectionId() { return this->projectionId; }
	GLuint getViewPosId() { return this->viewPosId; }
	GLuint getCubemapId() { return this->cubemapId; }
	void initUniforms();
	void switchProgram(mat4 Projection, mat4 Model, mat4 View, ViewSetup SetupTelecamera);
};

