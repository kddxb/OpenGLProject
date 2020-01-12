#pragma once
#include <GLM/glm.hpp>

struct Sphere
{
public:
	glm::vec3	center;			//ÇòĞÄ
	float		radius;			//°ë¾¶

	Sphere() {}

	Sphere(glm::vec3 center, float radius)
		:center(center), radius(radius)
	{}
};