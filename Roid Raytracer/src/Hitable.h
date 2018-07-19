#pragma once
#include <GLM\glm.hpp>
#include "Ray.h"
struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
};
class Hitable
{
public:
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const = 0;
};