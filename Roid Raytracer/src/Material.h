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
	float roughness;
	Metal(const glm::vec3& albedo, float roughness) { this->albedo = albedo; this->roughness = glm::clamp(roughness, 0.0f, 1.0f); }
	virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + roughness*randomInUnitSphere());
		attenuation = albedo;
		return (glm::dot(scattered.direction(), rec.normal) > 0);
	}
};
class Dielectric : public Material
{
public:
	float ref_idx;
	Dielectric(float ri) { ref_idx = ri; }
	virtual bool scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 outwardNormal;
		glm::vec3 reflected = glm::reflect(ray.direction(), rec.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 refracted;
		float reflect_prob;
		float cosine;
		if (glm::dot(ray.direction(), rec.normal) > 0)
		{
			outwardNormal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * glm::dot(ray.direction(), rec.normal) / ray.direction().length();
		}
		else
		{
			outwardNormal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -glm::dot(ray.direction(), rec.normal) / ray.direction().length();
		}
		if (refract(ray.direction(), outwardNormal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			//scattered = Ray(rec.p, reflected);
			reflect_prob = 1.0f;
		}
		if (random01() < reflect_prob)
		{
			scattered = Ray(rec.p, reflected);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}
};