#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

class TextCharacter
{
private:
	GLuint textureId;		// ID handle of the glyph texture
	ivec2 size;					// Size of glyph
	ivec2 bearing;				// Offset from baseline to left/top of glyph
	unsigned int advance;		// Horizontal offset to advance to next glyph
public:
	TextCharacter() {}
	TextCharacter(GLuint textureId, ivec2 size, ivec2 bearing, unsigned int advance);
	GLuint getTextureId() const { return this->textureId; }
	ivec2 getSize() const { return this->size; }
	ivec2 getBearing() const { return this->bearing; }
	unsigned int getAdvance() const { return this->advance; }
};

