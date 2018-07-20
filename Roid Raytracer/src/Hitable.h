#pragma once
#include <GLM\glm.hpp>
#include "Ray.h"

class Material;

struct HitRecord
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material* matPtr;
};
class Hitable
{
public:
	Material * matPtr;
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const = 0;
};