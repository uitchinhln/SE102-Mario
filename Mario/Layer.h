#pragma once
#include "Utils.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

class CLayer
{
	int id;

	int width;
	int height;

	int** tiles;
public:
	CLayer();
	CLayer(TiXmlElement* data);

	int GetTileID(int x, int y);

	~CLayer();
};

typedef CLayer* Layer;
