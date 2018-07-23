#pragma once
#include <GLM\glm.hpp>
#include "Ray.h"
class Camera
{
public:
	glm::vec3 origin;
	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	Camera(float vfov, float aspect);
	Ray getRay(float u, float v);
	~Camera();
};

