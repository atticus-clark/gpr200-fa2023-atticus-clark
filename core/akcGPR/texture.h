// texture.h
#pragma once

#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

unsigned int loadTexture(const char* filePath, GLint wrapMode, GLint filterMode, GLint filterModeMipmap);

/*
common wrapMode options: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
common filterMode options: GL_NEAREST, GL_LINEAR
common filterModeMipmap options: GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
*/