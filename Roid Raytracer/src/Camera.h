#pragma once
#include <GLM\glm.hpp>
#include "Ray.h"
class Camera
{
private:
	float vfov = 0;
	float aspect = 0;
public:
	glm::vec3 origin;
	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up,float vfov, float aspect);
	void UpdateTransform(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up);
	Ray getRay(float u, float v);
	~Camera();
};

