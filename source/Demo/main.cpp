#include "stdafx.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <jsoncpp/json.h>
#include <iostream>
#include <fstream>
#include "Program.h"

GLFWwindow* GetMainWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	GLFWwindow* window = GetMainWindow();
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	unsigned int indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	Program* pProgram = ProgramFactory::Get(ProgramType::Default);

	 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		pProgram->Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


GLFWwindow* GetMainWindow()
{
	std::ifstream inStream("config.json");
	Json::Reader reader;
	Json::Value root;
	reader.parse(inStream, root);
	int openglMajorVersion = root["openglVersion"]["major"].asInt();
	int openglMinorVersion = root["openglVersion"]["minor"].asInt();
	std::string initWindowTitle = root["window"]["initTitle"].asString();
	int initWindowWidth = root["window"]["initWidth"].asInt();
	int initWindowHeight = root["window"]["initHeight"].asInt();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(initWindowWidth, initWindowHeight, initWindowTitle.c_str(), nullptr, nullptr);
	return window;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}