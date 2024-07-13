#include "ProgramC.h"

void ProgramC::initUniforms()
{
	this->modelId = glGetUniformLocation(this->getProgramId(), "Model");
	this->projectionId = glGetUniformLocation(this->getProgramId(), "Projection");
	this->viewId = glGetUniformLocation(this->getProgramId(), "View");

	shaderId = glGetUniformLocation(this->getProgramId(), "sceltaVS");
	illuminationId = glGetUniformLocation(this->getProgramId(), "illumination");
	timeId = glGetUniformLocation(this->getProgramId(), "time");
	viewPosId = glGetUniformLocation(this->getProgramId(), "ViewPos");
	textureId = glGetUniformLocation(this->getProgramId(), "id_tex");

	// Light
	this->lightPositionId = glGetUniformLocation(this->getProgramId(), "light.position");
	this->lightColorId = glGetUniformLocation(this->getProgramId(), "light.color");
	this->lightPowerId = glGetUniformLocation(this->getProgramId(), "light.power");
	// Matrial
	this->materialAmbientId = glGetUniformLocation(this->getProgramId(), "material.ambient");
	this->materialDiffuseId = glGetUniformLocation(this->getProgramId(), "material.diffuse");
	this->materialSpecularId = glGetUniformLocation(this->getProgramId(), "material.specular");
	this->materialShininessId = glGetUniformLocation(this->getProgramId(), "material.shininess");
}

void ProgramC::switchProgram(mat4 Projection, mat4 View, ViewSetup SetupTelecamera, float time)
{
	Program::switchProgram();
	glPointSize(10.0);
	glUniformMatrix4fv(this->getProjectionId(), 1, GL_FALSE, value_ptr(Projection));
	glUniformMatrix4fv(this->getViewId(), 1, GL_FALSE, value_ptr(View));
	// Passo la posizione della camera
	glUniform3f(this->getViewPosId(), SetupTelecamera.position.x, SetupTelecamera.position.y, SetupTelecamera.position.z);
	glUniform1f(this->getTimeId(), time);
}

void ProgramC::switchScenaObject(mat4 Model, int shader, int illumination)
{
	glUniformMatrix4fv(this->getModelId(), 1, GL_FALSE, value_ptr(Model));
	glUniform1i(this->getShaderId(), shader);
	glUniform1i(this->getIlluminationId(), illumination);
}

void ProgramC::switchLight(LightPoint light, float angolo)
{
	glUniform3f(this->getLightPositionId(), light.position.x + 80 * cos(radians(angolo)), light.position.y, light.position.z + 80 * sin(radians(angolo)));
	glUniform3f(this->getLightColorId(), light.color.r, light.color.g, light.color.b);
	glUniform1f(this->getLightPowerId(), light.power);
}

void ProgramC::switchMaterial(Material material)
{
	glUniform3fv(this->getMaterialAmbientId(), 1, value_ptr(material.ambient));
	glUniform3fv(this->getMaterialDiffuseId(), 1, value_ptr(material.diffuse));
	glUniform3fv(this->getMaterialSpecularId(), 1, value_ptr(material.specular));
	glUniform1f(this->getMaterialShininessId(), material.shininess);
}
