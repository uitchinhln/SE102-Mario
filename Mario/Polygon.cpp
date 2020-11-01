#include "Polygon.h"

void CPolygon::BuildEdge()
{
	edges.clear();
	int n = vertices.size();
	for (int i = 0; i < n; i++) {
		edges[i] = vertices[i] - vertices[(i + 1) % n];
	}
}

void CPolygon::Project(Vec2 axis, float& min, float& max)
{
    float d = Vec2Utils::DotProduct(axis, vertices[0]);

	for each (Vec2 vertice in vertices)
	{
		d = Vec2Utils::DotProduct(axis, vertice); 
		
		if (d < min) {
			min = d;
		}
		else {
			if (d > max) {
				max = d;
			}
		}
	}
}

Vec2 CPolygon::Center()
{
	float totalX = 0;
	float totalY = 0;

	for each (Vec2 vertice in vertices)
	{
		totalX += vertice.x;
		totalY += vertice.y;
	}

	return Vec2(totalX / (float)vertices.size(), totalY / (float)vertices.size());
}
