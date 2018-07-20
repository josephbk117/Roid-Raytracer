#pragma once
#include "Hitable.h"
#include "Material.h"
#include <GLM\glm.hpp>
class Sphere : public Hitable
{
public:
	glm::vec3 center;
	float radius;
	Sphere() {}
	Sphere(glm::vec3 center, float r, Material* _mat) { this->center = center; radius = r; matPtr = _mat; }
	virtual bool hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const;
};
bool Sphere::hit(const Ray& ray, float t_min, float t_max, HitRecord& hitRecord)const
{
	glm::vec3 oc = ray.origin() - center;
	float a = glm::dot(ray.direction(), ray.direction());
	float b = glm::dot(oc, ray.direction());
	float c = glm::dot(oc, oc) - radius * radius;

	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - glm::sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			hitRecord.t = temp;
			hitRecord.p = ray.pointAtParameter(hitRecord.t);
			hitRecord.normal = (hitRecord.p - center) / radius;
			hitRecord.matPtr = matPtr;
			return true;
		}
		temp = (-b + glm::sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			hitRecord.t = temp;
			hitRecord.p = ray.pointAtParameter(hitRecord.t);
			hitRecord.normal = (hitRecord.p - center) / radius;
			hitRecord.matPtr = matPtr;
			return true;
		}
	}
	return false;
}