#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"

//uniform����������uniform��gl_VertexID��������VBO�е���������gl_PrimitiveID����ǰͼԪ��������ֻ����ƬԴ��ɫ����ʹ�ã�
int main06()
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


    unsigned int indices[] = {
        0,1,2,	//right bottom triangle
        2,3,0	//left top triangle
    };
    unsigned int VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    std::string vertexShader = { R"(
#version 430 core
uniform vec3 positions[4];
void main()
{
	gl_Position = vec4(positions[gl_VertexID], 1.0);
}
)" };
    std::string fragmentShader = { R"(
#version 430 core
uniform vec3 colors[2];
void main()
{
	gl_FragColor = vec4(colors[gl_PrimitiveID], 1);
}
)" };

    Program program(vertexShader, fragmentShader, false);
    program.Use();
    program.SetUniform("positions", std::vector<glm::vec3>{ glm::vec3(-0.5, -0.5, 0), glm::vec3(0.5, -0.5, 0), glm::vec3(0.5, 0.5, 0), glm::vec3(-0.5, 0.5, 0) });
    program.SetUniform("colors", std::vector<glm::vec3>{ glm::vec3(1, 0, 0), glm::vec3(0, 1, 0) });

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