#include "ProgramCubemap.h"

void ProgramCubemap::initUniforms()
{
	this->skyboxId = glGetUniformLocation(this->getProgramId(), "skybox");
	this->projectionId = glGetUniformLocation(this->getProgramId(), "Projection");
	this->viewId = glGetUniformLocation(this->getProgramId(), "View");
}

void ProgramCubemap::switchProgram(mat4 Projection, mat4 View)
{
	Program::switchProgram();
	glUniform1i(this->getSkyboxId(), 0);
	glUniformMatrix4fv(this->getProjectionId(), 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(this->getViewId(), 1, GL_FALSE, value_ptr(View));
}
