#pragma once

#include <GLM/glm.hpp>

class MathTool
{
public:
	//��ƽ���ͶӰ����
	static glm::mat4 ProjectionMatrixToPlane(glm::vec4 plane);

	//�㵽��ľ���
	static float DistanceOfPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n);

	//�������ϵ�ͶӰ
	static glm::vec3 ProjectPointToLine(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& direction);

	//����������һ�����ϵ�ͶӰ
	static glm::vec3 ProjectVecToVec(const glm::vec3& a, const glm::vec3& b);

	//�������ϵ�ͶӰ����
	static glm::vec3 ProjectionPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n);

	//ֱ������Ľ���
	static glm::vec3 IntersectOfLineAndPlane(const glm::vec3& lineBase, const glm::vec3& lineDir, const glm::vec3& planeBase, const glm::vec3& planeNormal);

	//�������·��غ�
	static bool IsSameVec(const glm::vec3& a, const glm::vec3& b, float delta = 1e-4);

	//Բ����
	static double PI();
};