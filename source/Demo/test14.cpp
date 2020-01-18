#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "Vertex.h"
#include "Mesh.h"
#include <GLM/gtx/transform.hpp>

//测试Mesh可用性
int main14()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(512, 512, u8"OpenGL学习", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::vector<Vertex> vertics = {
		Vertex(glm::vec3(-1,-1,0),glm::vec4(1,0,0,1)),
		Vertex(glm::vec3(1,-1,0),glm::vec4(0,1,0,1)),
		Vertex(glm::vec3(1,1,0),glm::vec4(0,0,1,1)),
		Vertex(glm::vec3(-1,1,0),glm::vec4(1,1,0,1)),
	};

	std::vector<glm::uvec3> indices = {
		glm::uvec3(0,1,2),
		glm::uvec3(2,3,0)
	};

	std::unique_ptr<Mesh> upMesh = Mesh::Create(vertics, indices);
	upMesh->SetCallBack([](Mesh* pMesh, float t) {
		pMesh->SetModelMatrix(glm::scale(glm::vec3(cos(t), sin(t), 1.0f)));
	});
	upMesh->Setup();

	Program* pProgram = ProgramFactory::Get(ProgramType::Mesh);
	pProgram->SetUniform("view", glm::mat4(1.0f));
	pProgram->SetUniform("projection", glm::mat4(1.0f));

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		upMesh->Update();
		upMesh->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}