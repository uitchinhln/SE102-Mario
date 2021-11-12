#pragma once
#include "Utils.h"
#include "Transform.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"
#include "Texture.h"

class CTileSet
{
	int firstgid;

	Vec2 tileSize;

	int tileCount;
	int columns;

	LPTEXTURE texture;

public:
	CTileSet(int firstgid, Vec2 tileSize, int tileCount, int columns, string imgPath);
	CTileSet(TiXmlElement* data, string xmlPath);

	int GetFirstGID();

	void Draw(int gid, float x, float y, Transform& transform, D3DXCOLOR overlay = D3DXCOLOR(255, 255, 255, 255));

	~CTileSet();
};

typedef CTileSet* TileSet;