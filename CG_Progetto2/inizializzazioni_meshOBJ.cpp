#include "ShaderMaker.h"
#include "Gestione_VAO.h"
#include "geometria.h"
#include "Strutture.h"
#include "GestioneMeshesAssImp.h"
#include "GestioneTexture.h"
#include "Program.h"
#include "ProgramC.h"
#include "ProgramCubemap.h"
#include "ProgramRiflessione.h"
#include "ProgramText.h"

extern vector<MeshObj> Model3D;
extern vector<vector<MeshObj>> ScenaObj;

extern ProgramC* programC;
extern ProgramCubemap* programS;
extern ProgramRiflessione* programR;
extern ProgramText* programText;
Mesh Cubo, Piano, Piramide;
extern vector<Mesh> Scena;
extern int height, width;
extern ViewSetup SetupTelecamera;
extern PerspectiveSetup SetupProspettiva;
extern vector<Material> materials;
extern vector<Shader> shaders;
extern vector<Illumination> illuminations;
extern LightPoint light;
extern int texture;

void initPrograms(void)
{
	// Program per gli oggetti della scena
	programC = new ProgramC();
	programC->initProgram("vertexShaderC.glsl", "fragmentShaderC.glsl");

	// Program per la cubemap
	programS = new ProgramCubemap();
	programS->initProgram("vertexShaderCubeMap.glsl", "fragmentShaderCubeMap.glsl");

	// Program per il testo
	programText = new ProgramText();
	programText->initProgram("VertexShaderText.glsl", "FragmentShaderText.glsl");

	// Program per la riflessione
	programR = new ProgramRiflessione();
	programR->initProgram("vertexShaderRiflessione.glsl", "fragmentShaderRiflessione.glsl");
}

// Clear the object
void clearObject(Mesh& oggetto) {
	oggetto.vertici.clear();
	oggetto.colori.clear();
	oggetto.indici.clear();
	oggetto.normali.clear();
	oggetto.texCoords.clear();
	oggetto.sceltaVS = 0;
}

// Crea l'oggetto dal file obj
void createObjectFromObj(vector<MeshObj> Model3D, string nome, int nmeshes, vec3 posizione, vec3 scala, float rad, vec3 rotation) {
	// Per ogni mesh dell'oggetto 3D creo un modello
	for (int i = 0; i < nmeshes; i++) {
		// Creo il VAO per la mesh
		crea_VAO_Vector_MeshObj(&Model3D[i]);
		Model3D[i].ModelM = mat4(1.0f);
		Model3D[i].ModelM = translate(Model3D[i].ModelM, posizione);
		Model3D[i].ModelM = scale(Model3D[i].ModelM, scala);
		if (rad != 0.0f) {
			Model3D[i].ModelM = rotate(Model3D[i].ModelM, radians(rad), rotation);
		}
		Model3D[i].nome = nome;
		Model3D[i].shader = ShaderType::INTERPOLATE_SHADING;
		Model3D[i].illumination = IlluminationType::BLINN;
		// Imposto il materiale
		vec3 ambiental = Model3D[i].materiale.ambient;
		vec3 difusivo = Model3D[i].materiale.diffuse;
		vec3 speculare = Model3D[i].materiale.specular;
		float shininess = Model3D[i].materiale.shininess;
		Model3D[i].materiale.ambient = ambiental;
		Model3D[i].materiale.diffuse = difusivo;
		Model3D[i].materiale.specular = speculare;
		Model3D[i].materiale.shininess = shininess;
		// Imposto lo shader
		Model3D[i].sceltaVS = 3;
	}
	// Inserisco il modello nella scena
	ScenaObj.push_back(Model3D);
}

void initVao(void)
{
	programText->initFreeType();
	programText->initVao();

	Mesh oggetto;
	int nmeshes;
	bool obj;

	// Skybox
	crea_skybox(&oggetto, vec4(0.0f, 0.0f, 0.0f, 0.0f));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	oggetto.Model = scale(oggetto.Model, vec3(1.0f, 1.0f, 1.0f));
	oggetto.nome = "Skybox";
	oggetto.shader = ShaderType::NO_SHADER;
	oggetto.material = MaterialType::NO_MATERIAL;
	oggetto.illumination = IlluminationType::NO_ILLUMINATION;
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// 4 Sfere per mostrare i diversi tipi di illuminazione e shader in tutte le combinazioni

	// Sfera 1
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-6.0, 0.0, 100.0));
	oggetto.Model = scale(oggetto.Model, vec3(5.0, 5.0, 5.0));
	oggetto.nome = "Interpolate Shading - Phong Illumination";
	oggetto.shader = ShaderType::INTERPOLATE_SHADING;
	oggetto.illumination = IlluminationType::PHONG;
	oggetto.material = MaterialType::EMERALD;
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// Sfera 2
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(-18.0, 0.0, 100.0));
	oggetto.Model = scale(oggetto.Model, vec3(5.0, 5.0, 5.0));
	oggetto.nome = "Interpolate Shading - Blinn-Phong Illumination";
	oggetto.shader = ShaderType::INTERPOLATE_SHADING;
	oggetto.illumination = IlluminationType::BLINN;
	oggetto.material = MaterialType::BRASS;
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// Sfera 3
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(6.0, 0.0, 100.0));
	oggetto.Model = scale(oggetto.Model, vec3(5.0, 5.0, 5.0));
	oggetto.nome = "Phong Shading - Phong Illumination";
	oggetto.shader = ShaderType::PHONG_SHADING;
	oggetto.illumination = IlluminationType::PHONG;
	oggetto.material = MaterialType::SLATE;
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// Sfera 4
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(18.0, 0.0, 100.0));
	oggetto.Model = scale(oggetto.Model, vec3(5.0, 5.0, 5.0));
	oggetto.nome = "Phong Shading - Blinn-Phong Illumination";
	oggetto.shader = ShaderType::PHONG_SHADING;
	oggetto.illumination = IlluminationType::BLINN;
	oggetto.material = MaterialType::EMERALD;
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// CUBO
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			crea_cubo(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
			crea_VAO_Vector(&oggetto);
			oggetto.Model = mat4(1.0);
			oggetto.Model = translate(oggetto.Model, vec3(35 + (i * 5), 0.0, 100.0 + (j * 5)));
			oggetto.Model = scale(oggetto.Model, vec3(5.0, 5.0, 5.0));
			oggetto.nome = "Cubo minecraft";
			oggetto.shader = ShaderType::TEXTURE;
			oggetto.illumination = IlluminationType::BLINN;
			oggetto.sceltaVS = loadTexture("spruce_log.png", 0);
			Scena.push_back(oggetto);
			clearObject(oggetto);
		}
	}

	// Sole
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(0.0, 50.0, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(10.0, 10.0, 10.0));
	oggetto.nome = "Sole";
	oggetto.shader = ShaderType::TEXTURE;
	oggetto.sceltaVS = loadTexture("8k_sun.jpg", 0);
	Scena.push_back(oggetto);
	clearObject(oggetto);

	// Luna
	crea_sfera(&oggetto, vec4(1.0, 0.0, 1.0, 1.0));
	crea_VAO_Vector(&oggetto);
	oggetto.Model = mat4(1.0);
	oggetto.Model = translate(oggetto.Model, vec3(70.0, 70.0, 0.0));
	oggetto.Model = scale(oggetto.Model, vec3(10.0, 10.0, 10.0));
	oggetto.nome = "Luna";
	oggetto.shader = ShaderType::TEXTURE;
	oggetto.sceltaVS = loadTexture("8k_moon.jpg", 0);
	Scena.push_back(oggetto);
	clearObject(oggetto);

	obj = loadAssImp("fischl/", "fischl.pmx", Model3D);
	nmeshes = Model3D.size();
	createObjectFromObj(Model3D, "Fischl", nmeshes, vec3(40.0, 2.5, 105.0), vec3(0.5, 0.5, 0.5), 180.0, vec3(0, 1, 0));
	Model3D.clear();

	obj = loadAssImp("barbara/", "barbara.pmx", Model3D);
	nmeshes = Model3D.size();
	createObjectFromObj(Model3D, "Barbara", nmeshes, vec3(50.0, 2.5, 105.0), vec3(0.5, 0.5, 0.5), 180.0, vec3(0, 1, 0));
	Model3D.clear();

	obj = loadAssImp("lumine/", "lumine.pmx", Model3D);
	nmeshes = Model3D.size();
	createObjectFromObj(Model3D, "Lumine", nmeshes, vec3(60.0, 2.5, 105.0), vec3(0.5, 0.5, 0.5), 180.0, vec3(0, 1, 0));
	Model3D.clear();

	obj = loadAssImp("", "ombrellone.obj", Model3D);
	nmeshes = Model3D.size();
	createObjectFromObj(Model3D, "Ombrellone", nmeshes, vec3(70.0, 2.5, 105.0), vec3(0.5, 0.5, 0.5), 0.0, vec3(1, 1, 1));
	Model3D.clear();
}

void initCameraProjection(void)
{
	// Imposto la telecamera
	SetupTelecamera.position = glm::vec4(0.0, 0.5, 150.0, 0.0);
	SetupTelecamera.target = glm::vec4(0.0, 0.0, 0.0, 0.0);
	SetupTelecamera.direction = SetupTelecamera.target - SetupTelecamera.position;
	SetupTelecamera.upVector = glm::vec4(0.0, 1.0, 0.0, 0.0);

	// Imposto la proiezione prospettica
	SetupProspettiva.aspect = (GLfloat)width / (GLfloat)height;
	SetupProspettiva.fovY = 45.0f;
	SetupProspettiva.far_plane = 2000.0f;
	SetupProspettiva.near_plane = 0.1f;
}

void initIlluminazione()
{
	light.position = { 0.0, 50.0, 0.0 };
	light.color = { 1.0, 1.0, 1.0 };
	light.power = 1.0f;

	// Setup dei materiali
	// Materials setup
	materials.resize(6);
	materials[MaterialType::RED_PLASTIC] = {
		"Red Plastic",
		vec3(0.1, 0.0, 0.0),
		vec3(0.6, 0.1, 0.1),
		vec3(0.7, 0.6, 0.6),
		32.0f
	};

	materials[MaterialType::EMERALD] = { 
		"Emerald", 
		vec3(0.0215, 0.04745, 0.0215),
		vec3(0.07568, 0.61424, 0.07568),
		vec3(0.633, 0.727811, 0.633),
		78.8f
	};

	materials[MaterialType::BRASS] = { 
		"Brass",
		vec3(0.1, 0.06, 0.015),
		vec3(0.78, 0.57, 0.11),
		vec3(0.99, 0.91, 0.81),
		27.8f
	};

	materials[MaterialType::SLATE] = { 
		"Slate",
		vec3(0.02, 0.02, 0.02),
		vec3(0.1, 0.1, 0.1),
		vec3(0.4, 0.4, 0.4),
		1.78125f
	};

	materials[MaterialType::YELLOW] = { 
		"Yellow",
		vec3(0.8, 0.8, 0.0),
		vec3(0.5, 0.5, 0.4),
		vec3(0.9, 0.9, 0.04),
		1.78125f
	};

	materials[MaterialType::NO_MATERIAL] = {
		"No Material",
		glm::vec3(1, 1, 1),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0),
		1.f
	};

	// Setup degli shader
	shaders.resize(4);
	shaders[ShaderType::NO_SHADER] = { "NO SHADER" };
	shaders[ShaderType::TEXTURE] = { "TEXTURE" };
	shaders[ShaderType::PHONG_SHADING] = { "PHONG SHADING" };
	shaders[ShaderType::INTERPOLATE_SHADING] = { "INTERPOLATE SHADING" };

	illuminations.resize(3);
	illuminations[IlluminationType::NO_ILLUMINATION] = { "NONE" };
	illuminations[IlluminationType::PHONG] = { "PHONG" };
	illuminations[IlluminationType::BLINN] = { "BLINN" };
}