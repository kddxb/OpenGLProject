#pragma once
#include <GLM/glm.hpp>

struct Vertex
{
	glm::vec3 position;		//顶点位置
	glm::vec3 normal;		//顶点法线
	glm::vec4 color;		//顶点颜色

	Vertex(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec4 color = glm::vec4(1, 1, 1, 1), glm::vec3 normal = glm::vec3(0, 0, 1))
		: position(position)
		, normal(normal)
		, color(color)
	{
	}

	//顶点属性
	enum VertexAttributeLocation
	{
		Position = 0,
		Normal = 1,
		Color = 2,
	};
};