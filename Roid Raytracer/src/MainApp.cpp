#include<iostream>
#include<GLM\glm.hpp>
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include "stb_image_write.h"
#include "Ray.h"
#include "TextureData.h"
#include "DrawingPanel.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"

GLFWwindow* window;

glm::vec3 color(const Ray& r, Hitable *world)
{
	HitRecord rec;
	if (world->hit(r, 0.0, 99999999.99f, rec))
	{
		return 0.5f * glm::vec3(rec.normal.x + 1, rec.normal.y + 1, rec.normal.z + 1);
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize(r.direction());
		float t = 0.5f * (unitDirection.y + 1.0);
		return (1.0f - t)*glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1200, 600, "Roid Raytracer", NULL, NULL);
	glfwSetWindowPos(window, 200, 200);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Open GL init error" << std::endl;
		return EXIT_FAILURE;
	}

	int const NX = 800;
	int const NY = 400;

	unsigned char *data = new unsigned char [NX * NY * 3];

	glm::vec3 lowerLeftCorner = glm::vec3(-2.0, -1.0, -1.0);
	glm::vec3 horizontal = glm::vec3(4.0, 0, 0);
	glm::vec3 vertical = glm::vec3(0, 2.0, 0);
	glm::vec3 origin = glm::vec3(0.0, 0.0, 0.0);
	
	Hitable *list[2];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5f);
	list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	for (int y = NY - 1; y >= 0; y--)
	{
		for (int x = 0; x < NX; x++)
		{
			int index = ((float)NX * (float)y + (float)x) * 3.0f;
			float u = (float)x / (float)NX;
			float v = (float)y / (float)NY;
			Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
			glm::vec3 col = color(ray, world);
			col *= 255;
			data[index] = col.r;
			data[index + 1] = col.g;
			data[index + 2] = col.b;
		}
	}

	//stbi_flip_vertically_on_write(true);
	//stbi_write_bmp("D:/rayImage.bmp", 200, 100, 3, data);

	TextureData texData;
	texData.setTextureData(data, NX, NY, 3);
	unsigned int textureId = TextureManager::loadTextureFromData(texData, false);

	DrawingPanel drawingPanel;
	drawingPanel.init(1, 1);
	drawingPanel.setTextureID(textureId);
	drawingPanel.getTransform()->setPosition(0, 0);
	drawingPanel.getTransform()->setScale(glm::vec2(1, 1));
	drawingPanel.getTransform()->update();

	ShaderProgram shader;
	shader.compileShaders("Resources\\spriteBase.vs", "Resources\\frameBuffer.fs");
	shader.linkShaders();
	shader.use();

	int drawingPanelModelMatrix = shader.getUniformLocation("model");
	
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.applyShaderUniformMatrix(drawingPanelModelMatrix, drawingPanel.getTransform()->getMatrix());
		drawingPanel.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}