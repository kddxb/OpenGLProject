#pragma once
#include <memory>

struct GLFWwindow;
class Camera;
class Program;

class OpenGLWindow
{
public:
	static GLFWwindow* Create();
	//窗口大小改变
	static void OnSize(GLFWwindow* window, int width, int height);
	//鼠标移动
	static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
	//鼠标按下与抬起
	static void OnMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	//滚轮事件
	static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset);
	//渲染
	static void RenderScene();

private:
	static std::shared_ptr<Camera> m_Camera;
	static Program* m_Program;
	static int m_LastMouseX;
	static int m_LastMouseY;
};