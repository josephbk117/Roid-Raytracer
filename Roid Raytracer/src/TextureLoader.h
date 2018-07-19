#pragma once
#include "TextureData.h"
#include <GL\glew.h>
#include <unordered_map>
#include <vector>
#include <GLM\common.hpp>
class TextureManager
{
public:
	TextureManager() = delete;
	TextureManager(const TextureManager&) = delete;
	static unsigned int loadTextureFromData(TextureData & textureData, bool gamma);
	static GLenum getTextureFormatFromData(TextureData & textureData);
	static GLenum getTextureFormatFromData(int componentCount);
};