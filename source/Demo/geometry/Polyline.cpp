#include "StdAfx.h"
#include "Polyline.h"

Polyline::Polyline(std::vector<Vertex> vertices)
	: m_Vertices(vertices)
{
}

std::unique_ptr<Polyline> Polyline::Create(std::vector<Vertex> vertices)
{
	if (vertices.empty())
	{
		return nullptr;
	}
	std::unique_ptr<Polyline> upPolyline(new Polyline(vertices));
	return std::move(upPolyline);
}