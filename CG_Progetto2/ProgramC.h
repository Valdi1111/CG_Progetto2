#pragma once
#include "Program.h"
#include "Strutture.h"

class ProgramC : public Program
{
protected:
	GLuint modelId;
	GLuint viewId;
	GLuint projectionId;

	GLuint shaderId;
	GLuint illuminationId;
	GLuint timeId;
	GLuint viewPosId;
	GLuint textureId;
	// Light
	GLuint lightPositionId;
	GLuint lightColorId;
	GLuint lightPowerId;
	// Matrial
	GLuint materialDiffuseId;
	GLuint materialAmbientId;
	GLuint materialSpecularId;
	GLuint materialShininessId;

public:
	ProgramC() {}

	GLuint getModelId() { return this->modelId; }
	GLuint getViewId() { return this->viewId; }
	GLuint getProjectionId() { return this->projectionId; }

	GLuint getShaderId() { return this->shaderId; }
	GLuint getIlluminationId() { return this->illuminationId; }
	GLuint getTimeId() { return this->timeId; }
	GLuint getViewPosId() { return this->viewPosId; }
	GLuint getTextureId() { return this->textureId; }

	GLuint getLightPositionId() { return this->lightPositionId; }
	GLuint getLightColorId() { return this->lightColorId; }
	GLuint getLightPowerId() { return this->lightPowerId; }

	GLuint getMaterialDiffuseId() { return this->materialDiffuseId; }
	GLuint getMaterialAmbientId() { return this->materialAmbientId; }
	GLuint getMaterialSpecularId() { return this->materialSpecularId; }
	GLuint getMaterialShininessId() { return this->materialShininessId; }
	
	void initUniforms();
	void switchProgram(mat4 Projection, mat4 View, ViewSetup SetupTelecamera, float time);
	void switchScenaObject(mat4 model, int shader, int illumination);
	void switchMaterial(Material material);
	void switchLight(LightPoint light, float angolo);
};

