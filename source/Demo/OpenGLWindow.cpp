#include "StdAfx.h"
#include "OpenGLWindow.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <jsoncpp/json.h>
#include <iostream>
#include <fstream>
#include <GLM/glm.hpp>
#include "Camera.h"
#include "Program.h"

std::shared_ptr<Camera> OpenGLWindow::m_Camera;
Program* OpenGLWindow::m_Program;
int OpenGLWindow::m_LastMouseX;
int OpenGLWindow::m_LastMouseY;

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

	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, &OnSize);
	glfwSetCursorPosCallback(window, &OnMouseMove);
	glfwSetMouseButtonCallback(window, &OnMouseButtonEvent);
	glfwSetScrollCallback(window, &OnMouseWheel);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	m_Camera = std::shared_ptr<Camera>(new Camera());

	m_Program = ProgramFactory::Get(ProgramType::Default);
	m_Program->Use();
	m_Program->SetUniform("model", glm::mat4(1));
	m_Program->SetUniform("view", glm::mat4(1));
	m_Program->SetUniform("projection", glm::mat4(1));

	return window;
}

void OpenGLWindow::OnSize(GLFWwindow* window, int width, int height)
{
	glm::ivec4 rect = glm::ivec4(0, 0, width, height);
	glViewport(rect.x, rect.y, rect.z, rect.w);
	m_Camera->SetViewPort(glm::vec4(rect.x, rect.y, rect.z, rect.w));
}

void OpenGLWindow::OnMouseMove(GLFWwindow * window, double xpos, double ypos)
{
	int winWidth, winHeight;
	glfwGetWindowSize(window, &winWidth, &winHeight);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glm::ivec2 offset = glm::ivec2(xpos - m_LastMouseX, ypos - m_LastMouseY);

		//如果给定旋转点与摄像机位置重合，特殊处理一下
		if (glm::distance(m_Camera->GetRotateCenter(), m_Camera->GetEye()) < 1e-6)
		{
			m_Camera->RotateViewX(glm::radians(offset.y * 0.5f));
			m_Camera->RotateViewY(glm::radians(offset.x * 0.5f));
		}
		else
		{
			m_Camera->RotateViewXByCenter(glm::radians(offset.y * 0.5f), m_Camera->GetRotateCenter());
			m_Camera->RotateViewYByCenter(glm::radians(offset.x * 0.5f), m_Camera->GetRotateCenter());
		}

		m_Camera->UpdateData();
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		//鼠标移动对应的世界坐标移动
		glm::vec3 pos0 = m_Camera->ScreenToWorld(glm::vec3(xpos, winWidth - ypos, 1));
		glm::vec3 pos1 = m_Camera->ScreenToWorld(glm::vec3(m_LastMouseX, winWidth - m_LastMouseY, 1));
		//移动相机
		m_Camera->MoveView(pos1 - pos0);
		m_Camera->UpdateData();
	}

	m_LastMouseX = xpos;
	m_LastMouseY = ypos;
}

void OpenGLWindow::OnMouseButtonEvent(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
		}
	}
	else if (action == GLFW_RELEASE)
	{
	}
}

void OpenGLWindow::OnMouseWheel(GLFWwindow * window, double xoffset, double yoffset)
{
	double xpos, ypos;
	int winWidth, winHeight;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &winWidth, &winHeight);

	float    present = yoffset > 0 ? 0.9f : 1.1f;
	glm::vec3 center = m_Camera->ScreenToWorld(glm::vec3(xpos, winWidth - ypos, 0));
	m_Camera->ScaleCameraByPos(center, present);
}

void OpenGLWindow::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//if (m_GeometryRender)
	{
		//更新相机数据
		m_Camera->UpdateData();

		m_Program->Use();
		//m_Program->SetUniform("model", glm::mat4(1));
		//m_Program->SetUniform("view", m_Camera->GetViewMatrix());
		//m_Program->SetUniform("projection", m_Camera->GetProjectionMatrix());

		m_Program->SetUniform("lightDirection", m_Camera->GetFront());
		m_Program->SetUniform("lightColor", glm::vec3(0.8, 0.8, 0.8));
		m_Program->SetUniform("viewPos", m_Camera->GetEye());

		m_Program->SetUniform("isLine", false);
		//m_GeometryRender->Draw();

		m_Program->SetUniform("isLine", true);
		//m_AxisRender->Draw();


		std::cout << m_Camera->GetFront().x << "  " << m_Camera->GetFront().y << "  " << m_Camera->GetFront().z << std::endl;
	}
}
