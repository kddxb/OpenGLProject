#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"

//一个VAO对应多个VBO
int main03()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(512, 512, u8"OpenGLѧϰ", nullptr, nullptr);
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

	float positions[] = {
		-1,-1,0,
		1,-1,0,
		1,1,0,
		-1,1,0,
	};

	float colors[] = {
		1,0,0,
		0,1,0,
		0,0,1,
		1,1,0,
	};

	float uvs[] = {
		0,0,
		1,0,
		1,1,
		0,1,
	};

	unsigned int indices[] = {
		0,1,2,	//right bottom triangle
		2,3,0	//left top triangle
	};


	enum
	{
		vbo_position,
		vbo_color,
		vbo_uv,
		vbo_count
	};

	unsigned int VAO, VBOs[vbo_count], EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(vbo_count, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vbo_position]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vbo_color]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[vbo_uv]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string vertexShader = { R"(
#version 430 core
layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vColor;
layout(location=2) in vec2 vUV;
out vec3 fColor;
out vec2 fUV;
void main()
{
	fColor = vColor;
	fUV = vUV * 2 - 1;
	gl_Position = vec4(vPosition, 1.0);
}
)" };
	std::string fragmentShader = { R"(
#version 430 core
in vec3 fColor;
in vec2 fUV;
uniform float t;
void main()
{
	float x = fUV.x;
	float y = fUV.y;
	float fx = 0.8 * sin(5 * x - t);
	float delta = abs(fx-y);
	if(delta < 0.03)
	{
		gl_FragColor = vec4(cos(x)/2 + 0.5, sin(y)/2 + 0.5, cos(t)/2 + 0.5, 1);
	}
	else
	{
		discard;
	}
}
)" };

	Program program(vertexShader, fragmentShader, false);

	glClearColor(0.8, 0.8, 0.8, 1.0);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		program.Use();
		program.SetUniform("t", float(glfwGetTime()));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}