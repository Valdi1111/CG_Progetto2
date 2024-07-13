#include "TextCharacter.h"

TextCharacter::TextCharacter(GLuint textureId, ivec2 size, ivec2 bearing, unsigned int advance) {
	this->textureId = textureId;
	this->size = size;
	this->bearing = bearing;
	this->advance = advance;
}
