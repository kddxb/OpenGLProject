#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "texture.h"
#include <memory>
#include <GLM/gtx/transform.hpp>

//ºÚµ•≤‚ ‘glmæÿ’Û±‰ªª
int main12()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(512, 512, u8"OpenGL—ßœ∞", nullptr, nullptr);
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

	float vertics[] = {
		-1,-1,0,	0,0,
		1,-1,0,		1,0,
		1,1,0,		1,1,
		-1,1,0,		0,1
	};

	unsigned int indices[] = {
		0,1,2,	//right bottom triangle
		2,3,0	//left top triangle
	};

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertics), vertics, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::unique_ptr<Texture2D> upTexture = Texture2D::Create("res/image/container.jpg", 0);

	std::string vertexShader = { R"(
#version 330 core
layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vUV;
uniform mat4 model;
out vec2 fUV;
void main()
{
	fUV = vUV;
	gl_Position = model * vec4(vPosition/2, 1.0);
}
)" };
	std::string fragmentShader = { R"(
#version 330 core
in vec2 fUV;
uniform sampler2D tex;
void main()
{
    gl_FragColor = texture(tex,fUV);
}
)" };
	Program program(vertexShader, fragmentShader, false);
	program.SetUniform("tex", upTexture.get());

	glClearColor(0.8, 0.8, 0.8, 1.0);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		program.Use();
		glm::mat4 model = glm::translate(glm::vec3(0.25, 0.25, 0));
		model = glm::rotate(model, float(glfwGetTime()), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-0.25, -0.25, 0));

		program.SetUniform("model", model);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}