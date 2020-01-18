#pragma once
#include "Geometry.h"
#include "Vertex.h"
#include <vector>
#include <memory>
#include <functional>

class Program;
enum class ProgramType;

class Mesh :public Geometry
{
private:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices, const glm::mat4& modelMatrix);

public:
	static std::unique_ptr<Mesh> Create(const std::vector<Vertex>& vertices, const std::vector<glm::uvec3>& indices, const glm::mat4& modelMatrix = glm::mat4(1.0f));
	void Setup();
	void Update();
	void Draw() const;
	ProgramType GetProgramType()const;
	Program* GetProgram()const;
	void SetProgramType(const ProgramType& programType);
	void SetCallBack(std::function<void(Mesh*, double)> callBack);

	const glm::mat4& GetModelMatrix() const;
	void SetModelMatrix(const glm::mat4& modelMatrix);
	void ResetModelMatrix();
	void Translate(const glm::vec3& v);
	void Rotate(float angle, const glm::vec3& axis);
	void Scale(const glm::vec3& v);
	void Transform(const glm::mat4& transformMatrix);

private:
	unsigned int m_VAO;
	std::vector<Vertex> m_Vertices;
	std::vector<glm::uvec3> m_Indices;
	glm::mat4 m_ModelMatrix;
	ProgramType m_ProgramType;
	std::function<void(Mesh*, double)> m_CallBack = nullptr;
};