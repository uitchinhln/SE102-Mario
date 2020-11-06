#pragma once
#include "Utils.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

class CGameMap;

class CLayer
{
	int id;
	int visible;

	int width;
	int height;

	int** tiles;
public:
	CLayer();
	CLayer(TiXmlElement* data, shared_ptr<CGameMap> map);

	bool Hidden = false;

	int GetTileID(int x, int y);

	~CLayer();
};

typedef CLayer* Layer;
