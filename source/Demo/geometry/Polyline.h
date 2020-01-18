#pragma once
#include "Geometry.h"
#include "Vertex.h"
#include <vector>
#include <memory>

class Polyline :public Geometry
{
private:
	Polyline(std::vector<Vertex> vertices);

public:
	static std::unique_ptr<Polyline> Create(std::vector<Vertex> vertices);

protected:
	std::vector<Vertex> m_Vertices;
};
