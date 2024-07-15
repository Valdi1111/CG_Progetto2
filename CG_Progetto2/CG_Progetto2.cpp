#include <iostream>

#include "ShaderMaker.h"
#include "Lib.h"
#include "geometria.h"

#include "Gestione_VAO.h"
#include "inizializzazioni_meshOBJ.h"
#include "GestioneInterazioni.h"
#include "GestioneTelecamera.h"
#include "GestioneMeshesAssImp.h"
#include "GestioneTexture.h"
#include <ft2build.h>
#include "Program.h"
#include "ProgramC.h"
#include "ProgramCubemap.h"
#include "ProgramRiflessione.h"
#include "ProgramText.h"
#include FT_FREETYPE_H

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

int last_mouse_pos_Y;
int last_mouse_pos_X;
bool moving_trackball = 0;
float angolo;
// Impostazione larghezza ed altezza della finestra sullo schermo
int width = 1024;
int height = 800;
// Definizione stringhe da visualizzare sulla finestra
string stringa_asse;
string Operazione;
vector<Material> materials;
vector<Shader> shaders;
vector<Illumination> illuminations;
vector<MeshObj> Model3D;
vector<vector<MeshObj>> ScenaObj;

int w_up = width;
int h_up = height;
mat4 Projection_text;
mat4 rotation_matrix = mat4(1.0);

ProgramC* programC;
ProgramCubemap* programS;
ProgramRiflessione* programR;
ProgramText* programText;
int idTex, texture, texture1, texture2, cubemapTexture;
float raggio_sfera = 2.5;
vec3 asse = vec3(0.0, 1.0, 0.0);
int selected_obj = 0;
float cameraSpeed = 0.5;
vector<Mesh> Scena, Snowman;
LightPoint light;

// Definzione SetupTelecamera e SetupProspettiva

ViewSetup SetupTelecamera;
PerspectiveSetup SetupProspettiva;

// Varibili per la gestione della direzione della telecamera tramite mouse
bool firstMouse = true;
float lastX = (float) width / 2;
float lastY = (float) height / 2;
float Theta = -90.0f;
float Phi = 0.0f;

mat4 Projection, Model, View;

void inizilizzaCubemap()
{
	vector<std::string> faces {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};
	cubemapTexture = loadCubemap(faces, 0);
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
//
void resize(int w, int h)
{
	// Imposto la matrice di Proiezione per il rendering del testo
	Projection_text = ortho(0.0f, (float)width, 0.0f, (float)height);

	// Imposto la matrice di proiezione per la scena da disegnare
	Projection = perspective(radians(SetupProspettiva.fovY), SetupProspettiva.aspect, SetupProspettiva.near_plane, SetupProspettiva.far_plane);

	float AspectRatio_mondo = (float)(width) / (float)(height); // Rapporto larghezza altezza di tutto ci� che � nel mondo
	// Se l'aspect ratio del mondo � diversa da quella della finestra devo mappare in modo diverso
	// per evitare distorsioni del disegno
	if (AspectRatio_mondo > w / h) // Se ridimensioniamo la larghezza della Viewport
	{
		glViewport(0, 0, w, w / AspectRatio_mondo);
		w_up = (float)w;
		h_up = w / AspectRatio_mondo;
	}
	else
	{ // Se ridimensioniamo la larghezza della viewport oppure se l'aspect ratio tra la finestra del mondo
		// e la finestra sullo schermo sono uguali
		glViewport(0, 0, h * AspectRatio_mondo, h);
		w_up = h * AspectRatio_mondo;
		h_up = (float)h;
	}
}

// Crea il menu per la scelta dei materiali, shader e illuminazione
void mainMenuHandler(int option)
{
	glutPostRedisplay();
}

void materialMenuHandler(int option)
{
	if (selected_obj > -1) {
		Scena[selected_obj].material = (MaterialType) option;
	}
	glutPostRedisplay();
}

void shaderMenuHandler(int option)
{
	if (selected_obj > -1) {
		Scena[selected_obj].shader = (ShaderType) option;
	}
	glutPostRedisplay();
}

void illuminationMenuHandler(int option) {
	if (selected_obj > -1) {
		Scena[selected_obj].illumination = (IlluminationType) option;
	}
	glutPostRedisplay();
}

void buildOpenGLMenu()
{
	// Aggiungo il menu material
	int materialSubMenu = glutCreateMenu(materialMenuHandler);
	for (size_t i = 0; i < materials.size(); i++)
	{
		glutAddMenuEntry(materials[i].name.c_str(), i);
	}

	// Aggiungo il menu shader
	int shaderSubMenu = glutCreateMenu(shaderMenuHandler);
	for (size_t i = 0; i < shaders.size(); i++)
	{
		glutAddMenuEntry(shaders[i].name.c_str(), i);
	}

	// Aggiungo il menu illumination
	int illuminationSubMenu = glutCreateMenu(illuminationMenuHandler);
	for (size_t i = 0; i < illuminations.size(); i++)
	{
		glutAddMenuEntry(illuminations[i].name.c_str(), i);
	}

	glutCreateMenu(mainMenuHandler); // richiama mainMenuHandler() alla selezione di una voce menu
	glutAddMenuEntry("Options", -1); // -1 significa che non si vuole gestire questa riga
	glutAddMenuEntry("---------------", -1);
	glutAddSubMenu("Material", materialSubMenu);
	glutAddSubMenu("Shader", shaderSubMenu);
	glutAddSubMenu("Illumination", illuminationSubMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void drawScene(void)
{
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	// Costruisco la matrice di Vista che applicata ai vertici in coordinate del mondo li trasforma nel sistema di riferimento della camera.
	View = lookAt(vec3(SetupTelecamera.position), vec3(SetupTelecamera.target), vec3(SetupTelecamera.upVector));

	// Skybox cube
	glDepthMask(GL_FALSE);
	programS->switchProgram(Projection, View);
	glBindVertexArray(Scena[0].VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, Scena[0].indici.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	// Riflessione
	programR->switchProgram(Projection, Scena[1].Model, View, SetupTelecamera);
	glBindVertexArray(Scena[1].VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, (Scena[1].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

	// Cambio program per renderizzare tutto il resto della scena
	programC->switchProgram(Projection, View, SetupTelecamera, time);

	// Definizione colore luce, posizione ed intensità
	programC->switchLight(light, angolo);

	for (size_t k = 1; k < Scena.size(); k++)
	{
		// Trasformazione delle coordinate dell'ancora dal sistema di riferimento dell'oggetto in sistema
		// di riferimento del mondo premoltiplicando per la matrice di Modellazione.
		Scena[k].ancora_world = Scena[k].ancora_obj;
		Scena[k].ancora_world = Scena[k].Model * Scena[k].ancora_world;

		programC->switchScenaObject(Scena[k].Model, Scena[k].shader, Scena[k].illumination);
		programC->switchMaterial(materials[Scena[k].material]);
		glBindVertexArray(Scena[k].VAO);

		if (Scena[k].sceltaVS != 0) {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, Scena[k].sceltaVS);
			glUniform1i(programC->getTextureId(), 1);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, (Scena[k].indici.size() - 1) * sizeof(GLuint), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	// Renderizzazione dei modelli obj
	for (size_t j = 0; j < ScenaObj.size(); j++)
	{
		for (size_t k = 0; k < ScenaObj[j].size(); k++)
		{
			programC->switchScenaObject(ScenaObj[j][k].ModelM, ScenaObj[j][k].shader, ScenaObj[j][k].illumination);
			programC->switchMaterial(ScenaObj[j][k].materiale);
			glBindVertexArray(ScenaObj[j][k].VAO);

			glDrawElements(GL_TRIANGLES, (ScenaObj[j][k].indici.size()) * sizeof(GLuint), GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}
	// Imposto il renderizzatore del testo
	if (selected_obj > -1) {
		programText->switchProgram(Projection_text);
		programText->renderText("Nome: " + Scena[selected_obj].nome, 20.0f, 770.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f));
		programText->renderText("Material: " + materials[Scena[selected_obj].material].name, 20.0f, 750.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f));
		programText->renderText("Shader: " + shaders[Scena[selected_obj].shader].name, 20.0f, 730.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f));
		programText->renderText("Illumination: " + illuminations[Scena[selected_obj].illumination].name, 20.0f, 710.0f, 0.5f, vec3(1.0f, 1.0f, 1.0f));
	}
	glutSwapBuffers();
}

void update(int value)
{
	glutTimerFunc(200, update, 0);
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// Inizializzo finestra per il rendering della scena 3d con tutti i suoi eventi le sue inizializzazioni e le sue impostazioni

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Progetto 2 CG");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);

	glutMouseFunc(handleMouse);
	glutKeyboardFunc(handleKeyboard);
	glutTimerFunc(200, update, 0);
	glutPassiveMotionFunc(handlePassiveMotion);
	glutMotionFunc(handleMotion);
	glewExperimental = GL_TRUE;

	// Inizializzazioni
	glewInit();
	initPrograms();
	initVao();
	initCameraProjection();
	initIlluminazione();
	buildOpenGLMenu();
	inizilizzaCubemap();
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	programC->initUniforms();
	programS->initUniforms();
	programR->initUniforms();
	programText->initUniforms();
	glutMainLoop();
}