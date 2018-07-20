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
bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt*(1.0f - dt * dt);
	if (discriminant > 0) 
	{
		refracted = ni_over_nt * (uv - n * dt) - n * glm::sqrt(discriminant);
		return true;
	}
	return false;
}
float schlick(float cosine, float ref_idx)
{
	float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 *= r0;
	return r0 + (1.0f - r0)*glm::pow((1.0f - cosine), 5);
}