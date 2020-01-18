#include "StdAfx.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Program.h"
#include "texture.h"
#include <memory>
#include <GLM/gtx/transform.hpp>

//mvp�任
int main13()
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

	unsigned int VAO_texture, VAO_line;
	{
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

		unsigned int VBO, EBO;
		glGenVertexArrays(1, &VAO_texture);
		glBindVertexArray(VAO_texture);

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
	}
	{
		float vertics[] = {
			0,0,0,	1,0,0,
			2,0,0,	1,0,0,
			0,0,0,	0,1,0,
			0,2,0,	0,1,0,
			0,0,0,	0,0,1,
			0,0,2,	0,0,1,
		};

		unsigned int VBO;
		glGenVertexArrays(1, &VAO_line);
		glBindVertexArray(VAO_line);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertics), vertics, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	std::unique_ptr<Texture2D> upTexture = Texture2D::Create("res/image/1234.png", 0);

	std::string textureVertexShader = { R"(
#version 430 core
layout(location=0) in vec3 vPosition;
layout(location=1) in vec2 vUV;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 fUV;
void main()
{
	fUV = vUV;
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}
)" };
	std::string textureFragmentShader = { R"(
#version 430 core
in vec2 fUV;
uniform sampler2D tex;
void main()
{
    gl_FragColor = texture(tex,fUV);
}
)" };

	std::string lineVertexShader = R"(
#version 430 core
layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 fColor;
void main()
{
	fColor = vColor;
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
}
)";
	std::string lineFragmentShader = R"(
#version 430 core
in vec3 fColor;
void main()
{
    gl_FragColor = vec4(fColor,1.0);
}
)";

	Program textureProgram(textureVertexShader, textureFragmentShader, false);
	textureProgram.SetUniform("tex", upTexture.get());
	Program lineProgram(lineVertexShader, lineFragmentShader, false);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glLineWidth(3);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);
	glm::mat4 model = glm::mat4(1);
	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			model = glm::mat4(1);
		}
		bool negative = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			model = glm::rotate(model, 0.001f, glm::vec3(negative ? -1 : 1, 0, 0));
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			model = glm::rotate(model, 0.001f, glm::vec3(0, negative ? -1 : 1, 0));
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			model = glm::rotate(model, 0.001f, glm::vec3(0, 0, negative ? -1 : 1));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(-0.001, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(0.001, 0, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(0, -0.001, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(0, 0.001, 0));
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(0, 0, -0.001));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			model = glm::translate(model, glm::vec3(0, 0, 0.001));
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f * width / height, 0.1f, 100.0f);
		{
			textureProgram.Use();
			textureProgram.SetUniform("model", model);
			textureProgram.SetUniform("view", view);
			textureProgram.SetUniform("projection", projection);

			glBindVertexArray(VAO_texture);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		{
			lineProgram.Use();
			lineProgram.SetUniform("model", model);
			lineProgram.SetUniform("view", view);
			lineProgram.SetUniform("projection", projection);

			glBindVertexArray(VAO_line);
			glDrawArrays(GL_LINES, 0, 6);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}