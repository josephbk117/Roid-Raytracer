#include "TextureLoader.h"
#include "TextureData.h"
#include <GL\glew.h>
#include <iostream>
unsigned int TextureManager::loadTextureFromData(TextureData & textureData, bool gamma)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	unsigned char* data = textureData.getTextureData();
	if (data)
	{
		GLenum format = GL_RED;
		if (textureData.getComponentCount() == 1)
			format = GL_RED;
		else if (textureData.getComponentCount() == 3)
			format = GL_RGB;
		else if (textureData.getComponentCount() == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, textureData.getWidth(), textureData.getHeight(), 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
#if _DEBUG
	else
		std::cout << "\nTexture failed to load with texture data "<< std::endl;
#endif
	return textureID;
}

GLenum TextureManager::getTextureFormatFromData(TextureData & textureData)
{
	GLenum format = GL_RED;
	if (textureData.getComponentCount() == 1)
		format = GL_RED;
	else if (textureData.getComponentCount() == 3)
		format = GL_RGB;
	else if (textureData.getComponentCount() == 4)
		format = GL_RGBA;
	return format;
}

GLenum TextureManager::getTextureFormatFromData(int componentCount)
{
	GLenum format = GL_RED;
	if (componentCount == 1)
		format = GL_RED;
	else if (componentCount == 3)
		format = GL_RGB;
	else if (componentCount == 4)
		format = GL_RGBA;
	return format;
}