#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "texture.h"
#include <memory>

//测试一维纹理封装类Texture1D
int main11()
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

    float vertics[] = {
        -1,-1,0,	0,
        1,-1,0,		1,
        1,1,0,		1,
        -1,1,0,		0,
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::vector<unsigned char> data;
	for (int i = 0; i < 255; ++i) { data.push_back(i); data.push_back(255 - i); data.push_back(255); }
	std::unique_ptr<Texture1D> upTexture = Texture1D::Create(data.data(), data.size() / 3, 3, 0);

	std::string vertexShader = { R"(
#version 430 core
layout(location=0) in vec3 vPosition;
layout(location=1) in float vU;
out float fU;
void main()
{
	fU = vU;
	gl_Position = vec4(vPosition/2, 1.0);
}
)" };
	std::string fragmentShader = { R"(
#version 430 core
in float fU;
uniform sampler1D tex;
void main()
{
    gl_FragColor = texture(tex,fU.s);
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

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}