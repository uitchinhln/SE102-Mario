#pragma once
#include "Utils.h"
#include "Transform.h"
#include "TinyXML/tinyxml.h"
#include "TinyXML/tinystr.h"

class CTileSet
{
	int firstgid;

	Vec2 tileSize;

	int tileCount;
	int columns;

	LPDIRECT3DTEXTURE9 texture;

public:
	CTileSet(int firstgid, Vec2 tileSize, int tileCount, int columns, string imgPath);
	CTileSet(TiXmlElement* data, string xmlPath);

	int GetFirstGID();

	void Draw(int gid, float x, float y, Transform& transform, D3DCOLOR overlay = D3DCOLOR_ARGB(255, 255, 255, 255));

	~CTileSet();
};

typedef CTileSet* TileSet;