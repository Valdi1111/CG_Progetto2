#pragma once

GLuint loadTexture(string name, int flip, bool append = true);
GLuint loadCubemap(vector<string> faces, int flip);