#pragma once
#include <GLM\common.hpp>
struct ColourData
{
private:
	glm::vec3 colour;
public:
	ColourData()
	{
		colour = glm::vec3(1.0f, 1.0f, 1.0f);
	}
	ColourData(float r, float g, float b)
	{
		colour = glm::vec3(r, g, b);
	}
	void setColour_8_Bit(const glm::vec4& colour)
	{
		int r = glm::clamp((int)colour.r, 0, 255);
		int g = glm::clamp((int)colour.g, 0, 255);
		int b = glm::clamp((int)colour.b, 0, 255);
		this->colour = glm::vec3(r, g, b);
	}
	void setColour_8_Bit(unsigned char r, unsigned char g, unsigned char b)
	{
		float _r = r / 255.0f;
		float _g = g / 255.0f;
		float _b = b / 255.0f;
		this->colour = glm::vec3(_r, _g, _b);
	}
	void setColour_32_bit(const glm::vec3& colour)
	{
		this->colour = colour;
	}
	void setColour_32_bit(float r, float g, float b)
	{
		this->colour = glm::vec3(r, g, b);
	}
	glm::vec3 getColourIn_0_1_Range()
	{
		return colour;
	}
	glm::vec3 getColour_8_Bit()
	{
		unsigned char rV = (unsigned char)(colour.r * 255.0f);
		unsigned char gV = (unsigned char)(colour.g * 255.0f);
		unsigned char bV = (unsigned char)(colour.b * 255.0f);
		return glm::vec3(rV, gV, bV);
	}
};