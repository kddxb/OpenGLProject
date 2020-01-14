#include "StdAfx.h"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <jsoncpp/json.h>
//#include <iostream>
//#include <fstream>
//#include <thread>
//#include <chrono>
//#include <functional>
//#include "Program.h"
//#include "Vertex.h"
//#include "OpenGLWindow.h"
//
//int main()
//{
//	GLFWwindow* window = OpenGLWindow::Create();
//	if (!window)
//	{
//		glfwTerminate();
//		return -1;
//	}
//
//	unsigned int VAO, VBO, EBO;
//	std::vector<Vertex> vertices = {
//		Vertex(glm::vec3(-0.5,-0.5,0),glm::vec3(1,0,0)),
//		Vertex(glm::vec3(0.5,-0.5,0),glm::vec3(0,1,0)),
//		Vertex(glm::vec3(0.5,0.5,0),glm::vec3(0,0,1)),
//		Vertex(glm::vec3(-0.5,0.5,0),glm::vec3(1,1,0)),
//	};
//
//	std::vector<glm::uvec3> indices = {
//		glm::uvec3(0,1,2),
//		glm::uvec3(2,3,0),
//	};
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &EBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3)*indices.size(), indices.data(), GL_STATIC_DRAW);
//
//	glVertexAttribPointer(Vertex::VertexAttributeLocation::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex,position)));
//	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Position);
//	glVertexAttribPointer(Vertex::VertexAttributeLocation::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
//	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Normal);
//	glVertexAttribPointer(Vertex::VertexAttributeLocation::Color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
//	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Color);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
//	while (!glfwWindowShouldClose(window))
//	{
//		OpenGLWindow::RenderScene();
//
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, static_cast<void*>(0));
//		glBindVertexArray(0);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwTerminate();
//	return 0;
//}