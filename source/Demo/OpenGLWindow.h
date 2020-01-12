#pragma once
#include <memory>

struct GLFWwindow;
class Camera;
class Program;

class OpenGLWindow
{
public:
	static GLFWwindow* Create();
	//���ڴ�С�ı�
	static void OnSize(GLFWwindow* window, int width, int height);
	//����ƶ�
	static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
	//��갴����̧��
	static void OnMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
	//�����¼�
	static void OnMouseWheel(GLFWwindow* window, double xoffset, double yoffset);
	//��Ⱦ
	static void RenderScene();

private:
	static std::shared_ptr<Camera> m_Camera;
	static Program* m_Program;
	static int m_LastMouseX;
	static int m_LastMouseY;
};