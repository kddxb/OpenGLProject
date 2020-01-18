#include "StdAfx.h"
#include "MathTool.h"
#include <glm\gtx\transform.hpp>

glm::mat4 MathTool::ProjectionMatrixToPlane(glm::vec4 plane)
{
	glm::mat4 ret;
	float div = plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2];
	ret[0][0] = -plane[0] * plane[0] / div + 1;
	ret[0][1] = -plane[0] * plane[1] / div;
	ret[0][2] = -plane[0] * plane[2] / div;
	ret[0][3] = -plane[0] * plane[3] / div;
	ret[1][0] = -plane[1] * plane[0] / div;
	ret[1][1] = -plane[1] * plane[1] / div + 1;
	ret[1][2] = -plane[1] * plane[2] / div;
	ret[1][3] = -plane[1] * plane[3] / div;
	ret[2][0] = -plane[2] * plane[0] / div;
	ret[2][1] = -plane[2] * plane[1] / div;
	ret[2][2] = -plane[2] * plane[2] / div + 1;
	ret[2][3] = -plane[2] * plane[3] / div;
	ret[3][0] = 0;
	ret[3][1] = 0;
	ret[3][2] = 0;
	ret[3][3] = 1;
	return ret;
}

float MathTool::DistanceOfPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n)
{
	return glm::length(ProjectVecToVec(basePoint - point, n));
}

glm::vec3 MathTool::ProjectPointToLine(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& direction)
{
	return basePoint + ProjectVecToVec(point - basePoint, direction);
}

glm::vec3 MathTool::ProjectVecToVec(const glm::vec3 & a, const glm::vec3 & b)
{
	return (glm::dot(a, b) / glm::dot(b, b))*b;
}

glm::vec3 MathTool::ProjectionPointToPlane(const glm::vec3& point, const glm::vec3& basePoint, const glm::vec3& n)
{
	return  point + ProjectVecToVec(basePoint - point, n);
}

glm::vec3 MathTool::IntersectOfLineAndPlane(const glm::vec3& lineBase, const glm::vec3& lineDir, const glm::vec3& planeBase, const glm::vec3& planeNormal)
{
	float d = glm::dot(planeBase - lineBase, planeNormal) / glm::dot(glm::normalize(lineDir), planeNormal);
	return d * glm::normalize(lineDir) + lineBase;
}

bool MathTool::IsSameVec(const glm::vec3& a, const glm::vec3& b, float delta /*= 1e-4*/)
{
	return glm::length(a - b) < delta;
}

double MathTool::PI()
{
	return 3.141592653;
}