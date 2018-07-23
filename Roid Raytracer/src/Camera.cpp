#include "Camera.h"
#include <GLM\gtc\constants.hpp>

Camera::Camera(float vfov, float aspect)
{
	float theta = vfov * glm::pi<float>() / 180.0f;
	float half_height = glm::tan(theta / 2.0f);
	float half_width = aspect * half_height;
	lowerLeftCorner = glm::vec3(-half_width, -half_height, -1.0f);
	horizontal = glm::vec3(2*half_width, 0,0);
	vertical = glm::vec3(0, 2 * half_height, 0);
	origin = glm::vec3(0);
}

Ray Camera::getRay(float u, float v)
{
	return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
}
Camera::~Camera()
{
}
