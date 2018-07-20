#pragma once
#include <GLM\glm.hpp>
#include "Ray.h"
#include "Hitable.h"
#include "MathUtils.h"
class Material
{
public:
	virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered)const = 0;
};
class Lambertian : public Material
{
public:
	glm::vec3 albedo;
	Lambertian(const glm::vec3& albedo) { this->albedo = albedo; }
	virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 target = rec.p + rec.normal + randomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
};
class Metal : public Material
{
public:
	glm::vec3 albedo;
	Metal(const glm::vec3& albedo) { this->albedo = albedo; }
	virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction()), rec.normal);
		scattered = Ray(rec.p, reflected);
		attenuation = albedo;
		return (glm::dot(scattered.direction(), rec.normal) > 0);
	}
};