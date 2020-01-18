#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "Vertex.h"
#include "Mesh.h"
#include <GLM/gtx/transform.hpp>
#include "MeshUtils.h"

//测试创建长方体，球体，圆柱
int main15()
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

	std::vector<std::unique_ptr<Mesh>> meshes;

	std::unique_ptr<Mesh> upCuboid = MeshUtils::CreateAsCuboid(glm::vec3(0.5, 0.5, 0.5), glm::vec4(1, 1, 0, 1));
	upCuboid->SetCallBack([](Mesh* pMesh, float t) {
		glm::mat4 model = glm::translate(glm::vec3(-0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, t, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
		pMesh->SetModelMatrix(model);
	});
	meshes.push_back(std::move(upCuboid));

	std::unique_ptr<Mesh> upSphere = MeshUtils::CreateAsSphere(0.5f, glm::vec4(1, 0, 0, 1));
	upSphere->SetCallBack([](Mesh* pMesh, float t) {
		glm::mat4 model = glm::scale(glm::vec3(cos(t), sin(t), cos(t) + sin(t)));
		pMesh->SetModelMatrix(model);
	});
	meshes.push_back(std::move(upSphere));

	std::unique_ptr<Mesh> upCylinder = MeshUtils::CreateAsCylinder(0.25f, 1.0f, glm::vec4(0, 1, 0, 1), glm::rotate(glm::radians(45.0f), glm::vec3(1, 1, 1)));
	meshes.push_back(std::move(upCylinder));
	for (auto& mesh : meshes)
	{
		mesh->Setup();
	}

	Program* pProgram = ProgramFactory::Get(ProgramType::Mesh);
	pProgram->SetUniform("view", glm::mat4(1.0f));
	pProgram->SetUniform("projection", glm::mat4(1.0f));

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& mesh : meshes)
		{
			mesh->Update();
			mesh->Draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}