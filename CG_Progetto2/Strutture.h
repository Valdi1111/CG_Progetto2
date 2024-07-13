#pragma once
#include "Lib.h"
#include "enum_illuminazione.h"
#include <string>

typedef struct
{
	string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
} Material;

typedef struct
{
	vector<vec3> vertici;
	vector<vec4> colori;
	vector<GLuint> indici;
	vector<vec3> normali;
	vector<vec2> texCoords;
	mat4 Model;
	int sceltaVS;
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint VBO_normali;
	GLuint EBO_indici;
	GLuint VBO_coord_texture;
	string nome;
	IlluminationType illumination;
	MaterialType material;
	ShaderType shader;
	vec4 ancora_obj;
	vec4 ancora_world;
} Mesh;

typedef struct
{
	vector<vec3> vertici;
	vector<vec4> colori;
	vector<GLuint> indici;
	vector<vec3> normali;
	vector<vec2> texCoords;
	mat4 ModelM;
	int sceltaVS;
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint VBO_normali;
	GLuint EBO_indici;
	GLuint VBO_coord_texture;
	IlluminationType illumination;
	ShaderType shader;
	Material materiale;
	string nome;
	vec4 ancora_obj;
	vec4 ancora_world;
} MeshObj;

// Gestione Vista

typedef struct
{
	vec4 position;
	vec4 target;
	vec4 upVector;
	vec4 direction;
} ViewSetup;

// gestione proiezione
typedef struct
{
	float fovY, aspect, near_plane, far_plane;
} PerspectiveSetup;

typedef struct
{
	string name;
} Shader;

typedef struct
{
	string name;
} Illumination;

typedef struct
{
	glm::vec3 position;
	glm::vec3 color;
	GLfloat power;
} LightPoint;
