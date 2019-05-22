#include<iostream>
#include<GLM\glm.hpp>
#include<GL\glew.h>
#include<GLFW\glfw3.h>
#include <thread>

#include "stb_image_write.h"
#include "Ray.h"
#include "TextureData.h"
#include "DrawingPanel.h"
#include "ShaderProgram.h"
#include "TextureLoader.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "MathUtils.h"
#include "Material.h"
#include "Camera.h"

#define ANTI_ALIASING

GLFWwindow* window;
const float MAX_FLOAT = 9999999999.9999f;

glm::vec3 color(const Ray& r, Hitable *world, int depth)
{
	HitRecord rec;
	if (world->hit(r, 0.001, MAX_FLOAT, rec))
	{
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return glm::vec3(0, 0, 0);
		}
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize(r.direction());
		float t = 0.5f * (unitDirection.y + 1.0);
		return (1.0f - t)*glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
	}
}

void performRaytraceSection(const int NX, const int NY, const int NS, int from, int to, int y, unsigned char *data, Hitable *world, Camera camera)
{
	for (int x = from; x < to; x++)
	{
		glm::vec3 col(0.0f, 0.0f, 0.0f);
		int index = ((float)NX * (float)y + (float)x) * 3.0f;
#ifdef ANTI_ALIASING
		for (int s = 0; s < NS; s++)
		{
			float u = (float)((x + random01()) / (float)NX);
			float v = (float)((y + random01()) / (float)NY);
			Ray ray = camera.getRay(u, v); //Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
			col += color(ray, world, 0);
		}
		col /= float(NS);
#else
		float u = (float)x / (float)NX;
		float v = (float)y / (float)NY;
		Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
		col = color(ray, world, 0);
#endif
		col = glm::vec3(glm::sqrt(col.r), glm::sqrt(col.g), glm::sqrt(col.b));
		col *= 255;
		data[index] = col.r;
		data[index + 1] = col.g;
		data[index + 2] = col.b;
	}
}

void performRaytrace(const int NX, const int NY, const int NS, int from, int to, unsigned char *data, Hitable *world, Camera camera)
{
	for (int y = from; y >= to; y--)
	{
		performRaytraceSection(NX, NY, NS, 0, NX, y, data, world, camera);
	}
}

int main()
{
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1400, 700, "Roid Raytracer", NULL, NULL);
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

	const int NX = 380;
	const int NY = 190;
	const int NS = 40;

	unsigned char *data = new unsigned char[NX * NY * 3];

	Camera camera(glm::vec3(-2, 2, -1), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 45, (float)NX / (float)NY);

	Hitable *list[5];
	list[0] = new Sphere(glm::vec3(0, 0, -1), 0.5f, new Lambertian(glm::vec3(0.8, 0.3, 0.3)));
	list[1] = new Sphere(glm::vec3(0, -100.5, -1), 100, new Lambertian(glm::vec3(0.8, 0.8, 0.0)));
	list[2] = new Sphere(glm::vec3(1, 0, -1), 0.4f, new Metal(glm::vec3(0.2, 0.5, 0.5), 0.85f));
	list[3] = new Sphere(glm::vec3(-1, 0, -1), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(glm::vec3(-1, 0, -1), -0.495f, new Dielectric(1.5f));
	Hitable *world = new HitableList(list, 5);

	//	for (int y = NY - 1; y >= 0; y--)
	//	{
	//		for (int x = 0; x < NX; x++)
	//		{
	//			glm::vec3 col(0.0f, 0.0f, 0.0f);
	//			int index = ((float)NX * (float)y + (float)x) * 3.0f;
	//#ifdef ANTI_ALIASING
	//			for (int s = 0; s < NS; s++)
	//			{
	//				float u = (float)((x + random01()) / (float)NX);
	//				float v = (float)((y + random01()) / (float)NY);
	//				Ray ray = camera.getRay(u, v); //Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
	//				col += color(ray, world, 0);
	//		}
	//			col /= float(NS);
	//#else
	//			float u = (float)x / (float)NX;
	//			float v = (float)y / (float)NY;
	//			Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
	//			col = color(ray, world,0);
	//#endif
	//			col = glm::vec3(glm::sqrt(col.r), glm::sqrt(col.g), glm::sqrt(col.b));
	//			col *= 255;
	//			data[index] = col.r;
	//			data[index + 1] = col.g;
	//			data[index + 2] = col.b;
	//	}
	//}

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
	float rVal = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(64.0f / 255.0f, 75.0f / 255.0f, 105.0f / 255.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.applyShaderUniformMatrix(drawingPanelModelMatrix, drawingPanel.getTransform()->getMatrix());
		drawingPanel.draw();
		camera.UpdateTransform(glm::vec3(-2, 2, glm::sin(rVal) * 2), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		rVal += 0.1f;
		int sections = NY / 6;
		std::thread t1(performRaytrace, NX, NY, NS, sections, 0, data, world, camera);
		std::thread t2(performRaytrace, NX, NY, NS, sections * 2, sections, data, world, camera);
		std::thread t3(performRaytrace, NX, NY, NS, sections * 3, sections * 2, data, world, camera);
		std::thread t4(performRaytrace, NX, NY, NS, sections * 4, sections * 3, data, world, camera);
		std::thread t5(performRaytrace, NX, NY, NS, sections * 5, sections * 4, data, world, camera);
		std::thread t6(performRaytrace, NX, NY, NS, sections * 6, sections * 5, data, world, camera);
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		t5.join();
		t6.join();
		//performRaytrace(NX, NY, NS, NY, 0, data, world, camera);
		/*for (int y = NY - 1; y >= 0; y--)
		{*/
		//performRaytraceSection(NX, NY, NS, 0, NX, y, data, world, camera);
		/*std::thread t1(performRaytraceSection,NX, NY, NS, 0, sections, y, data, world, camera);
		std::thread t2(performRaytraceSection, NX, NY, NS, sections, sections * 2, y, data, world, camera);
		std::thread t3(performRaytraceSection, NX, NY, NS, sections * 2, sections * 3, y, data, world, camera);
		std::thread t4(performRaytraceSection, NX, NY, NS, sections * 3, sections * 4, y, data, world, camera);
		t1.join();
		t2.join();
		t3.join();
		t4.join();*/
		//			for (int x = 0; x < NX; x++)
		//			{
		//				glm::vec3 col(0.0f, 0.0f, 0.0f);
		//				int index = ((float)NX * (float)y + (float)x) * 3.0f;
		//#ifdef ANTI_ALIASING
		//				for (int s = 0; s < NS; s++)
		//				{
		//					float u = (float)((x + random01()) / (float)NX);
		//					float v = (float)((y + random01()) / (float)NY);
		//					Ray ray = camera.getRay(u, v); //Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
		//					col += color(ray, world, 0);
		//				}
		//				col /= float(NS);
		//#else
		//				float u = (float)x / (float)NX;
		//				float v = (float)y / (float)NY;
		//				Ray ray(origin, lowerLeftCorner + u * horizontal + v * vertical);
		//				col = color(ray, world, 0);
		//#endif
		//				col = glm::vec3(glm::sqrt(col.r), glm::sqrt(col.g), glm::sqrt(col.b));
		//				col *= 255;
		//				data[index] = col.r;
		//				data[index + 1] = col.g;
		//				data[index + 2] = col.b;
		//}
	//}
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, NX, NY, TextureManager::getTextureFormatFromData(3), GL_UNSIGNED_BYTE, data);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteTextures(1, &textureId);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}