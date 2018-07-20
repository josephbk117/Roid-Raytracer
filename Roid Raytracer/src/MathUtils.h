#pragma once
#include <GLM\glm.hpp>
float random01()
{
	return (rand() % 9999) / 9999.0f;
}

glm::vec3 randomInUnitSphere()
{
	glm::vec3 p;
	p = glm::vec3(random01(), random01(), random01());
	p = glm::normalize(p);
	return p;
}