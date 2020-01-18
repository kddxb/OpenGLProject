#include "StdAfx.h"
#include "Point.h"

Point::Point(const Vertex & vertex)
	: m_Vertex(vertex)
{
}

std::unique_ptr<Point> Point::Create(const Vertex& vertex)
{
	std::unique_ptr<Point> upPoint(new Point(vertex));
	return std::move(upPoint);
}
