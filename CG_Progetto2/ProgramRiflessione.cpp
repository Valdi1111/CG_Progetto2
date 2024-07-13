#include "ProgramRiflessione.h"

void ProgramRiflessione::initUniforms()
{
	this->modelId = glGetUniformLocation(this->getProgramId(), "Model");
	this->viewId = glGetUniformLocation(this->getProgramId(), "View");
	this->projectionId = glGetUniformLocation(this->getProgramId(), "Projection");
	this->viewPosId = glGetUniformLocation(this->getProgramId(), "ViewPos");
	this->cubemapId = glGetUniformLocation(this->getProgramId(), "cubemap");
}

void ProgramRiflessione::switchProgram(mat4 Projection, mat4 Model, mat4 View, ViewSetup SetupTelecamera)
{
	Program::switchProgram();
	glUniformMatrix4fv(this->getProjectionId(), 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(this->getModelId(), 1, GL_FALSE, value_ptr(Model));
	glUniformMatrix4fv(this->getViewId(), 1, GL_FALSE, value_ptr(View));
	glUniform3f(this->getViewPosId(), SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
}