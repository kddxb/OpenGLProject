#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"

//glDrawElementsBaseVertex实现动画效果
int main07()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(512, 512, u8"OpenGL学习", nullptr, nullptr);
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

	std::vector<float> vertices;
	for (int i = -10; i <= 10; ++i)
	{
		std::vector<float> tmp = {
			-0.5f + 0.05f*i, -0.5f, 0,	1,0,0,
			0.5f + 0.05f*i, -0.5f,  0,	0,1,0,
			0.5f + 0.05f*i,  0.5f,  0,	0,0,1,
			-0.5f + 0.05f*i,  0.5f, 0,	1,1,1
		};
		vertices.insert(vertices.end(), tmp.begin(), tmp.end());
	}

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

	unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::string vertexShader = { R"(
#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
out vec3 fColor;
void main()
{
    fColor = vColor;
    gl_Position = vec4(vPos, 1.0);
}
)" };

    std::string fragmentShader = { R"(
#version 330 core
out vec4 FragColor;
in vec3 fColor;
void main()
{
    FragColor = vec4(fColor, 1.0);
}
)" };

    Program program(vertexShader, fragmentShader, false);
    program.Use();

    glClearColor(0.8, 0.8, 0.8, 1.0);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        program.Use();
        glBindVertexArray(VAO);
		glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, (4 * int(glfwGetTime())) % (vertices.size() / 6));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}