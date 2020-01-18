#pragma once
#include <GLM/glm.hpp>

struct Vertex
{
	glm::vec3 position;		//����λ��
	glm::vec3 normal;		//���㷨��
	glm::vec4 color;		//������ɫ

	Vertex(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec4 color = glm::vec4(1, 1, 1, 1), glm::vec3 normal = glm::vec3(0, 0, 1))
		: position(position)
		, normal(normal)
		, color(color)
	{
	}

	//��������
	enum VertexAttributeLocation
	{
		Position = 0,
		Normal = 1,
		Color = 2,
	};
};