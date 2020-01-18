#include "StdAfx.h"
#include "MeshUtils.h"
#include "Mesh.h"

namespace
{
	const float PI = 3.141592653f;
}

std::unique_ptr<Mesh> MeshUtils::CreateAsCuboid(const glm::vec3 & size, const glm::vec4& color, const glm::mat4 & modelMatrix)
{
	if (size.x <= 0 || size.y <= 0 || size.z <= 0)
	{
		return nullptr;
	}

	float halfSizeX = size.x * 0.5f;
	float halfSizeY = size.y * 0.5f;
	float halfSizeZ = size.z * 0.5f;

	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(-halfSizeX,  -halfSizeY,  +halfSizeZ),color),
		Vertex(glm::vec3(+halfSizeX,  -halfSizeY,  +halfSizeZ),color),
		Vertex(glm::vec3(+halfSizeX,  +halfSizeY,  +halfSizeZ),color),
		Vertex(glm::vec3(-halfSizeX,  +halfSizeY,  +halfSizeZ),color),
		Vertex(glm::vec3(-halfSizeX,  -halfSizeY,  -halfSizeZ),color),
		Vertex(glm::vec3(+halfSizeX,  -halfSizeY,  -halfSizeZ),color),
		Vertex(glm::vec3(+halfSizeX,  +halfSizeY,  -halfSizeZ),color),
		Vertex(glm::vec3(-halfSizeX,  +halfSizeY,  -halfSizeZ),color),
	};

	std::vector<glm::uvec3> indices = {
		glm::uvec3(1, 0, 4),
		glm::uvec3(1, 4, 5),
		glm::uvec3(2, 1, 5),
		glm::uvec3(2, 5, 6),
		glm::uvec3(3, 2, 6),
		glm::uvec3(3, 6, 7),
		glm::uvec3(0, 3, 7),
		glm::uvec3(0, 7, 4),
		glm::uvec3(6, 5, 4),
		glm::uvec3(6, 4, 7),
		glm::uvec3(0, 1, 2),
		glm::uvec3(3, 0, 2)
	};

	return Mesh::Create(vertices, indices, modelMatrix);
}

std::unique_ptr<Mesh> MeshUtils::CreateAsSphere(float radius, const glm::vec4& color, const glm::mat4 & modelMatrix, int rows, int columns)
{
	if (radius <= 0)
	{
		return nullptr;
	}

	std::vector<Vertex> vertices;
	std::vector<glm::uvec3> indices;

	for (int i = 0; i < (rows + 1) * (columns + 1); i++)
	{
		float x = float(i % (columns + 1));
		float y = float(i / (columns + 1));
		float x_pos = x / columns;
		float y_pos = y / rows;
		vertices.emplace_back(glm::vec3(x_pos, y_pos, 0), color);
	}

	for (int i = 0; i < 2 * rows * columns; i++)
	{
		int triIndex[3] = { 0 };
		if (i % 2 == 0)
		{
			triIndex[0] = i / 2 + i / (2 * columns);
			triIndex[1] = triIndex[0] + 1;
			triIndex[2] = triIndex[0] + (columns + 1);
		}
		else
		{
			triIndex[0] = (i + 1) / 2 + i / (2 * columns);
			triIndex[1] = triIndex[0] + (columns + 1);
			triIndex[2] = triIndex[1] - 1;
		}
		indices.emplace_back(triIndex[0], triIndex[1], triIndex[2]);
	}

	for (int i = 0; i < int(vertices.size()); i++)
	{
		glm::vec3 v;
		v.x = float(radius * cos(vertices[i].position.x * 2 * PI) * cos(vertices[i].position.y * PI - PI / 2));
		v.y = float(radius * sin(vertices[i].position.x * 2 * PI) * cos(vertices[i].position.y * PI - PI / 2));
		v.z = float(radius * sin(vertices[i].position.y * PI - PI / 2));
		vertices[i] = Vertex(v, color);
		vertices[i].normal = glm::normalize(vertices[i].position);
	}

	return Mesh::Create(vertices, indices, modelMatrix);
}

std::unique_ptr<Mesh> MeshUtils::CreateAsCylinder(float radius, float height, const glm::vec4 & color, const glm::mat4 & modelMatrix,int parts)
{
	if (radius <= 0 || height <= 0)
	{
		return nullptr;
	}
	std::vector<Vertex> vertices;
	std::vector<glm::uvec3> indices;

	glm::vec3 basePoint = glm::vec3(0, 0, 0);
	glm::vec3 topPoint = glm::vec3(0, 0, height);
	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);

	float delta = 2 * PI / parts;

	vertices.resize(2 * parts);
	for (int i = 0; i < parts; i++)
	{
		float angle = delta * i;
		{
			vertices[i] = Vertex(radius * cos(angle) * x + radius * sin(angle) * y, color);
			glm::vec3 normal = glm::normalize(vertices[i].position - basePoint);
			vertices[i].normal = normal;
		}
		{
			vertices[i + parts] = Vertex(topPoint + radius * cos(angle) * x + radius * sin(angle) * y, color);
			glm::vec3 normal = glm::normalize(vertices[i + parts].position - topPoint);
			vertices[i + parts].normal = normal;
		}
	}

	for (int i = 0; i <= parts; i++)
	{
		indices.push_back(glm::uvec3(i % parts, (i + 1) % parts, parts + i % parts));
		indices.push_back(glm::uvec3((i + 1) % parts, parts + (i + 1) % parts, parts + i % parts));
	}

	vertices.resize(4 * parts + 2);
	for (int i = 2 * parts; i < 4 * parts; i++)
	{
		vertices[i] = vertices[i - 2 * parts];
		if (i < 3 * parts)
		{
			vertices[i].normal = -z;
		}
		else
		{
			vertices[i].normal = z;
		}
	}
	vertices[4 * parts + 0] = Vertex(glm::vec3(0, 0, 0), color);
	vertices[4 * parts + 0].normal = -z;
	vertices[4 * parts + 1] = Vertex(topPoint, color);
	vertices[4 * parts + 1].normal = z;

	for (int i = 0; i <= parts; i++)
	{
		indices.push_back(glm::uvec3(4 * parts + 0, 2 * parts + i%parts, 2 * parts + (i + 1) % parts));
		indices.push_back(glm::uvec3(4 * parts + 1, 3 * parts + i%parts, 3 * parts + (i + 1) % parts));
	}

	return Mesh::Create(vertices, indices, modelMatrix);
}