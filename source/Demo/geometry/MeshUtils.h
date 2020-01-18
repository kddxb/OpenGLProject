#pragma once
#include <memory>
#include <GLM/glm.hpp>

class Mesh;

class MeshUtils
{
public:
	//���������壬ԭ��������
	static std::unique_ptr<Mesh> CreateAsCuboid(const glm::vec3& size, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f));
	//�������壬ԭ��������
	static std::unique_ptr<Mesh> CreateAsSphere(float radius, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f), int rows = 25, int columns = 50);
	//����Բ����ԭ���ڵײ�����
	static std::unique_ptr<Mesh> CreateAsCylinder(float radius, float height, const glm::vec4& color, const glm::mat4& modelMatrix = glm::mat4(1.0f), int parts = 60);
};