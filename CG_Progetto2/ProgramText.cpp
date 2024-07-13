#include "ProgramText.h"
#include <iostream>
#include <vector>
#include <ft2build.h>
#include "TextCharacter.h"
#include FT_FREETYPE_H

using namespace std;
using namespace glm;

void ProgramText::initFreeType()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
		exit(-1);
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		printf("ERROR::FREETYPE: Failed to load font\n");
		exit(-1);
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				printf("ERROR::FREETYTPE: Failed to load Glyph \n");
				continue;
			}
			// generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			/*TextCharacter character = TextCharacter::TextCharacter();*/
			TextCharacter character = TextCharacter::TextCharacter(
				texture,
				ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			);
			charactersMap.insert(pair<char, TextCharacter>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void ProgramText::initUniforms()
{
	this->projectionId = glGetUniformLocation(this->getProgramId(), "textPosition");
	this->textColorId = glGetUniformLocation(this->getProgramId(), "textColor");
}

void ProgramText::switchProgram(mat4 projection)
{
	Program::switchProgram();
	glUniformMatrix4fv(this->projectionId, 1, GL_FALSE, value_ptr(projection));
}

void ProgramText::initVao()
{
	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ProgramText::renderText(string text, float x, float y, float scale, vec3 color)
{
	// activate corresponding render state	
	glUniform3f(this->textColorId, color.x, color.y, color.z);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->vao);

	// iterate through all characters
	for (auto c = text.begin(); c != text.end(); c++)
	{
		TextCharacter ch = charactersMap[*c];

		float xpos = x + ch.getBearing().x * scale;
		float ypos = y - (ch.getSize().y - ch.getBearing().y) * scale;

		float w = ch.getSize().x * scale;
		float h = ch.getSize().y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.getTextureId());
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.getAdvance() >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
