#include "StdAfx.h"
#include "TimeTool.h"
#include <GLFW/glfw3.h>

double TimeTool::GetTime()
{
	return glfwGetTime();
}