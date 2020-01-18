#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "Vertex.h"
#include "Mesh.h"
#include <GLM/gtx/transform.hpp>
#include "MeshUtils.h"
#include <map>

//窗体宽高
int width = 600, height = 800;
//方块尺寸
int cellSize = 30;
int rows = 15, columns = 10;




struct Cell
{
	int row;
	int column;
	int index;
};

struct Temp
{
	std::vector<Cell> cells;
};

int main16()
{
#pragma region MyRegion
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, u8"OpenGL学习", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, width, height);
		glfwSetWindowSize(window, width, height);
	});

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
#pragma endregion

	//缓存所有方块
	std::vector<std::unique_ptr<Mesh>> allCubes;
	//根据索引获取方块在vector中的索引
	std::map<int, std::map<int, int>> cubeGrid;
	//索引与位置的关系
	std::function<glm::vec3(int, int)> getModelMatrixByIndex = [=](int row, int column)->glm::vec3 {
		return glm::vec3(cellSize * (column + 0.5), cellSize * (row + 0.5), 0);
	};

	//根据
	std::function<void(Mesh*, int, int)>updateMesh = [=](Mesh* pMesh, int row, int column) {
		pMesh->ResetModelMatrix();
		pMesh->Translate(getModelMatrixByIndex(row, column));
	};

	//产生一个系列






	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			std::unique_ptr<Mesh> upCuboid = MeshUtils::CreateAsCuboid(glm::vec3(1, 1, 1) * (cellSize * 0.9f), glm::vec4(0, 1, 0, 1));
			upCuboid->Translate(getModelMatrixByIndex(y, x) + glm::vec3(0, 0, -cellSize));
			allCubes.push_back(std::move(upCuboid));
		}
	}

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < columns; ++x)
		{
			std::unique_ptr<Mesh> upCuboid = MeshUtils::CreateAsCuboid(glm::vec3(1, 1, 1) * (cellSize * 0.7f), glm::vec4(1, 1, 0, 1));
			upCuboid->Translate(getModelMatrixByIndex(y, x));
			//upCuboid->SetVisible(false);
			allCubes.push_back(std::move(upCuboid));
			cubeGrid[y][x] = allCubes.size() - 1;
		}
	}

	for (auto& cube : allCubes)
	{
		cube->Setup();
	}

	Program* pProgram = ProgramFactory::Get(ProgramType::Mesh);
	glm::vec3 eye = glm::vec3(0.5f * width, 0.5f * height, 50);
	glm::vec3 front = glm::vec3(0, 0, -1);
	pProgram->SetUniform("view", glm::lookAt(eye, eye + front, glm::vec3(0, 1, 0)));

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pProgram->SetUniform("projection", glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, 0.1f, 100.0f));

		for (auto& cube : allCubes)
		{
			if (cube->IsVisible())
			{
				cube->Update();
				cube->Draw();
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}