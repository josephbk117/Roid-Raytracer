#pragma once
#include "ColourData.h"
class TextureData
{
private:
	unsigned char* data;
	int width;
	int height;
	int componentCount;
public:
	TextureData();
	void setTextureData(unsigned char* data, int width, int height, int componentCount);
	unsigned char* getTextureData();
	const int getWidth()noexcept;
	const int getHeight()noexcept;
	int getComponentCount()noexcept;
	void setTexelColor(int r, int g, int b, int x, int y);
	void setTexelColor(ColourData& colourData, int x, int y);
	ColourData getTexelColor(int x, int y);

	~TextureData();
};

