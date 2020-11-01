#pragma once
#include "Vec2Utils.h"

class CPolygon
{
public:
	vector<Vec2> vertices;
	vector<Vec2> edges;

	void BuildEdge();

	void Project(Vec2 axis, float& min, float& max);

	Vec2 Center();
};

