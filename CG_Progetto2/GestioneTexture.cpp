#include <iostream>
#include "Lib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SkyboxDir "SkyBox/"
#define Texturesdir "Textures/"

map<string, GLuint> loadedTextures;

GLuint loadTexture(string name, int flip, bool append = true)
{
	// Carica la texture solo se non già caricata in precedenza
	auto it = loadedTextures.find(name);
	if (it != loadedTextures.end()) {
		return loadedTextures[name];
	}
	string path = name;
	if (append) {
		path = Texturesdir + name;
	}
	int width, height, channels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		printf("Texture failed to load at path: %s\n", path.c_str());
		stbi_image_free(data);
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	printf("Loading texture %s - numero di canali %d\n", path.c_str(), channels);

	GLenum format{};
	if (channels == 1) {
		format = GL_RED;
	}
	else if (channels == 3) {
		format = GL_RGB;
	}
	else if (channels == 4) {
		format = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	loadedTextures[name] = textureID;
	return textureID;
}

GLuint loadCubemap(vector<string> faces, int flip)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (size_t i = 0; i < faces.size(); i++)
	{
		string path = SkyboxDir + faces[i];
		printf("Loading cubemap %d\n", i);
		stbi_set_flip_vertically_on_load(flip);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("Cubemap tex failed to load at path: %s\n", path.c_str());
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}