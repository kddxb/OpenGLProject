#include "StdAfx.h"
#include "OpenGLWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <jsoncpp/json.h>
#include <iostream>
#include <fstream>

void OpenGLWindow::OnSizeChange(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* OpenGLWindow::Create()
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

	glfwSetFramebufferSizeCallback(window, &OpenGLWindow::OnSizeChange);

	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, &OnSizeChange);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}