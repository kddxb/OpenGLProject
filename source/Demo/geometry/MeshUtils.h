#pragma once
#include <memory>
#include <GLM/glm.hpp>

class Mesh;

class MeshUtils
{
public:
	//创建长方体，原点在中心
	static std::unique_ptr<Mesh> CreateAsCuboid(const glm::vec3& size, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f));
	//创建球体，原点在球心
	static std::unique_ptr<Mesh> CreateAsSphere(float radius, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f), int rows = 25, int columns = 50);
	//创建圆柱，原点在底部中心
	static std::unique_ptr<Mesh> CreateAsCylinder(float radius, float height, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f), int parts = 60);
};