#include "stdafx.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <jsoncpp/json.h>
#include <iostream>
#include <fstream>
#include "Program.h"
#include <thread>
#include <chrono>
#include "Vertex.h"

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

	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-0.5,-0.5,0),glm::vec3(1,0,0)),
		Vertex(glm::vec3(0.5,-0.5,0),glm::vec3(0,1,0)),
		Vertex(glm::vec3(0.5,0.5,0),glm::vec3(0,0,1)),
		Vertex(glm::vec3(-0.5,0.5,0),glm::vec3(1,1,0)),
	};

	std::vector<glm::uvec3> indices = {
		glm::uvec3(0,1,2),
		glm::uvec3(2,3,0),
	};


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)*indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(Vertex::VertexAttributeLocation::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,position)));
	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Position);
	glVertexAttribPointer(Vertex::VertexAttributeLocation::Color, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	Program* defaultProgram = ProgramFactory::Get(ProgramType::Default);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		defaultProgram->Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, static_cast<void*>(0));
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