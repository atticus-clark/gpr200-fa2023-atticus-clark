// texture.cpp

#include "texture.h"

unsigned int loadTexture(const char* filePath, GLint wrapMode, GLint filterMode, GLint filterModeMipmap)
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);

	if(data == NULL)
	{
		printf("Failed to load image %s\n", filePath);
		stbi_image_free(data);
		return 0;
	}

	// choose internal format and image format depending on numComponents
	GLint internalFormat;
	GLenum imageFormat;
	switch(numComponents)
	{
	case 1:
		internalFormat = GL_RED;
		imageFormat = GL_RED;
		break;
	case 2:
		internalFormat = GL_RG;
		imageFormat = GL_RG;
		break;
	case 3:
		internalFormat = GL_RGB;
		imageFormat = GL_RGB;
		break;
	case 4:
		internalFormat = GL_RGBA;
		imageFormat = GL_RGBA;
		break;
	default:
		printf("Texture could not be generated; invalid value for %s",
			"numComponents (should be 1, 2, 3, or 4)\n");
		stbi_image_free(data);
		return 0;
		break;
	}

	unsigned int texture;
	glGenTextures(1, &texture); // create texture name
	glBindTexture(GL_TEXTURE_2D, texture); // bind/create texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
		imageFormat, GL_UNSIGNED_BYTE, data); // reserve memory and set texture data

	// set wrapping and filtering modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterModeMipmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	return texture;
}