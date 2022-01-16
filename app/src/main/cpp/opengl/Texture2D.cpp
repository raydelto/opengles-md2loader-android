#include "Texture2D.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "Logger.h"


using namespace Raydelto::MD2Loader;

Texture2D::Texture2D()
	: mTexture(0)
{
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mTexture);
}

bool Texture2D::loadTexture(const string &fileName, bool generateMipMaps)
{
	int width, height, components;

	// Use stbi image library to load our image
	unsigned char *imageData = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha);

	if (imageData == nullptr)
	{
		LOGE("Error loading texture %s\n", fileName.c_str());
		return false;
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
	// unbinding our texture
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}

void Texture2D::bind(GLuint texUnit) const
{
	assert(texUnit >= 0 && texUnit < 32);
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}
