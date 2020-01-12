#pragma once

struct GLFWwindow;

class OpenGLWindow
{
public:
	static void OnSizeChange(GLFWwindow* window, int width, int height);
	static GLFWwindow* Create();
};