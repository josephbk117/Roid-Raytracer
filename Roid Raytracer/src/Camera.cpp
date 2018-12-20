#include "Camera.h"
#include <GLM\gtc\constants.hpp>

Camera::Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up, float vfov, float aspect)
{
	glm::vec3 u, v, w;
	this->vfov = vfov;
	this->aspect = aspect;

	float theta = vfov * glm::pi<float>() / 180.0f;
	float half_height = glm::tan(theta / 2.0f);
	float half_width = aspect * half_height;

	origin = lookFrom;
	w = glm::normalize(lookFrom - lookAt);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w, u);

	lowerLeftCorner = glm::vec3(-half_width, -half_height, -1.0f);
	lowerLeftCorner = origin - half_width * u - half_height * v - w;
	horizontal = 2.0f * half_width * u;
	vertical = 2.0f * half_height * v;
}

void Camera::UpdateTransform(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 up)
{
	glm::vec3 u, v, w;

	float theta = this->vfov * glm::pi<float>() / 180.0f;
	float half_height = glm::tan(theta / 2.0f);
	float half_width = this->aspect * half_height;

	origin = lookFrom;
	w = glm::normalize(lookFrom - lookAt);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w, u);

	lowerLeftCorner = glm::vec3(-half_width, -half_height, -1.0f);
	lowerLeftCorner = origin - half_width * u - half_height * v - w;
	horizontal = 2.0f * half_width * u;
	vertical = 2.0f * half_height * v;
}

Ray Camera::getRay(float u, float v)
{
	return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
}
Camera::~Camera()
{
}
