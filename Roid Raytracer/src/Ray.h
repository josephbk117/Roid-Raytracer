#pragma once
#include <GLM\glm.hpp>
class Ray
{
public:
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 origin()const { return A; }
	glm::vec3 direction()const { return B; }
	glm::vec3 pointAtParameter(float t)const { return A + t * B; }
	Ray();
	Ray(const glm::vec3& a, const glm::vec3& b)
	{
		A = a;
		B = b;
	}
	~Ray();
};

