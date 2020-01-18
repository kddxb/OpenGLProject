#pragma once
#include "Geometry.h"
#include "Vertex.h"
#include <memory>

class Point :public Geometry
{
private:
	Point(const Vertex& vertex);

public:
	static std::unique_ptr<Point> Create(const Vertex& vertex);

protected:
	Vertex m_Vertex;
};