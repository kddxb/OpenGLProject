#include "StdAfx.h"
#include "Mesh.h"
#include <algorithm>
#include <GLAD/glad.h>
#include "Program.h"
#include <glm/gtx/transform.hpp>
#include "TimeTool.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices, const glm::mat4& modelMatrix)
	: m_Vertices(vertices)
	, m_Indices(indices)
	, m_ModelMatrix(modelMatrix)
	, m_ProgramType(ProgramType::Mesh)
{
}

std::unique_ptr<Mesh> Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices, const glm::mat4& modelMatrix)
{
	if (vertices.empty() || indices.empty())
	{
		return nullptr;
	}
	unsigned int maxIndice = 0;
	for (const glm::uvec3& indice : indices)
	{
		maxIndice = std::max({ maxIndice, indice.x, indice.y, indice.z });
		if (maxIndice > vertices.size())
		{
			return nullptr;
		}
	}

	std::unique_ptr<Mesh> upGeometryMesh(new Mesh(vertices, indices, modelMatrix));
	return std::move(upGeometryMesh);
}

void Mesh::Setup()
{
    if (m_IsSetup)
    {
        return;
    }
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices.front(), GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(glm::uvec3), &m_Indices.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(Vertex::VertexAttributeLocation::Position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, position)));
	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Position);
	glVertexAttribPointer(Vertex::VertexAttributeLocation::Normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Normal);
	glVertexAttribPointer(Vertex::VertexAttributeLocation::Color, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
	glEnableVertexAttribArray(Vertex::VertexAttributeLocation::Color);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_IsSetup = true;
}

void Mesh::Update()
{
	if (m_CallBack)
	{
		m_CallBack(this, TimeTool::GetTime());
	}
}

void Mesh::Draw() const
{
	Program* pProgram = GetProgram();
	pProgram->SetUniform("model", m_ModelMatrix);
	pProgram->Use();
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indices.size() * 3, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

ProgramType Mesh::GetProgramType() const
{
	return m_ProgramType;
}

Program * Mesh::GetProgram() const
{
	return ProgramFactory::Get(m_ProgramType);;
}

void Mesh::SetProgramType(const ProgramType & programType)
{
	m_ProgramType = programType;
}

void Mesh::SetCallBack(std::function<void(Mesh*, double)> callBack)
{
	m_CallBack = callBack;
}

const glm::mat4 & Mesh::GetModelMatrix() const
{
	return m_ModelMatrix;
}

void Mesh::SetModelMatrix(const glm::mat4& modelMatrix)
{
	m_ModelMatrix = modelMatrix;
}

void Mesh::ResetModelMatrix()
{
	m_ModelMatrix = glm::mat4(1.0f);
}

void Mesh::Translate(const glm::vec3 & v)
{
	glm::mat4 translateMatrix = glm::translate(v);
	Transform(translateMatrix);
}

void Mesh::Rotate(float angle, const glm::vec3& axis)
{
	glm::mat4 rotateMatrix = glm::rotate(angle, axis);
	Transform(rotateMatrix);
}

void Mesh::Scale(const glm::vec3 & v)
{
	glm::mat4 scaleMatrix = glm::scale(v);
	Transform(scaleMatrix);
}

void Mesh::Transform(const glm::mat4 & transformMatrix)
{
	m_ModelMatrix = transformMatrix * m_ModelMatrix;
}