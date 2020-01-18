#pragma once

#include <GLM/glm.hpp>

class MathTool
{
public:
	//到平面的投影矩阵
	static glm::mat4 ProjectionMatrixToPlane(glm::vec4 plane);

	//点到面的距离
	static float DistanceOfPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n);

	//点在线上的投影
	static glm::vec3 ProjectPointToLine(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& direction);

	//向量在另外一向量上的投影
	static glm::vec3 ProjectVecToVec(const glm::vec3& a, const glm::vec3& b);

	//点在面上的投影坐标
	static glm::vec3 ProjectionPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n);

	//直线与面的交点
	static glm::vec3 IntersectOfLineAndPlane(const glm::vec3& lineBase, const glm::vec3& lineDir, const glm::vec3& planeBase, const glm::vec3& planeNormal);

	//两个点事否重合
	static bool IsSameVec(const glm::vec3& a, const glm::vec3& b, float delta = 1e-4);

	//圆周率
	static double PI();
};